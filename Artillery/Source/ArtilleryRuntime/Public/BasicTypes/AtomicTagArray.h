#pragma once

#include <atomic>
#include <memory>
#include <utility>
#include "NativeGameplayTags.h"
#include "SkeletonTypes.h"

#include "CoreMinimal.h"
#include "Templates/SubclassOf.h"
#include "UObject/UnrealType.h"
#include "Engine/DataTable.h"
#include "Containers/CircularBuffer.h"

THIRD_PARTY_INCLUDES_START
PRAGMA_PUSH_PLATFORM_DEFAULT_PACKING
#include "libcuckoo/cuckoohash_map.hh"
PRAGMA_POP_PLATFORM_DEFAULT_PACKING
THIRD_PARTY_INCLUDES_END

#include "AtomicTagArray.generated.h"

struct FConservedTagContainer;
typedef TSharedPtr<FGameplayTagContainer> FS_GameplayTagPtr;
typedef TSharedPtr<TArray<FGameplayTag>> FTagLayer;
#define FAST_TAG_MAX_C 30
typedef TMap<FGameplayTag, uint16_t> UnderlyingTagMapping;
typedef TMap<uint16_t, FGameplayTag> UnderlyingTagReverse;
typedef TSharedPtr<TMap<FGameplayTag, uint16_t>> TagsSeen;
typedef TSharedPtr<UnderlyingTagReverse> TagsByCode;

typedef TWeakPtr<UnderlyingTagReverse> Flimsy;

//TODO: add tag rollback in Artillery Dispatch
//TODO: add save-all or copy-all for the tag representations themselves.
//we don't need the cuckoo hash for rollback, since you can no longer remove or add entities to saved frames.
//In fact, we technically don't need the atomic either, but it's best to keep the exact TagStateRepresentation.
//if we switch to true tombstoning, we may be able to change erase so that it only deletes the mapping.
//that would simplify this design some.
//It seems likely that a modified generational allocator might actually point a way to success here by allowing us to just copy
//BLOBBO and to get some better memory contiguity at the same time. Practical max burn here is gonna be 10 megs, which is
//enough that we could use permanent pooled blocks per tick and avoid all fragmentation as well as closing out ticks trivially once they
//can't be rolled back.

//Tag rollback is easier than it looks.
//We actually just save either the final or starting representation for every entity on a per tick basis.
//During the StackUp sequence, or another fixed point in the tick, we save ALL state reps
//in fact, ideally, we save the whole ATA, though it may be better
struct FTagStateRepresentation
{
	uint16_t Tags[FAST_TAG_MAX_C];
	std::atomic_uint32_t snagged = 0;
	TWeakPtr<FConservedTagContainer> AccessRefController;
	
	FTagStateRepresentation() : Tags{}
	{
	}
	
	//This allows a held conserved tag container to be used directly
	bool Find(uint16 Numerology);
	bool Remove(uint16 Numerology);
	bool Add(uint16 Numerology);
};

typedef TSharedPtr<FTagStateRepresentation> FTagsPtr;
using FConservedTags = TSharedPtr<FConservedTagContainer> ;
THIRD_PARTY_INCLUDES_START
PRAGMA_PUSH_PLATFORM_DEFAULT_PACKING
typedef libcuckoo::cuckoohash_map<uint32_t, FTagsPtr> Entities;
typedef libcuckoo::cuckoohash_map<uint32_t, FS_GameplayTagPtr> SlowEntities;
PRAGMA_POP_PLATFORM_DEFAULT_PACKING
THIRD_PARTY_INCLUDES_END

//the fetching tag container hides the existence of the AtomicTagArray from UE as a whole as best we can.
USTRUCT(BlueprintType)
struct ARTILLERYRUNTIME_API FConservedTagContainer 
{
	GENERATED_BODY()
	
	friend class UArtilleryDispatch;
	friend class AtomicTagArray;
	friend class FArtilleryBusyWorker;
	FConservedTagContainer() = default;
	virtual ~FConservedTagContainer() = default;
	friend class AtomicTagArray;
	constexpr static uint8 RollbackFrames = 10;

	virtual FTagLayer GetFrameByNumber(uint64_t FrameNumber);
	virtual bool Find(FGameplayTag Bot);
	FConservedTags GetReference();
	TSharedPtr<TArray<FGameplayTag>> GetAllTags();
	TSharedPtr<TArray<FGameplayTag>> GetAllTags(uint64_t FrameNumber);
	virtual bool Remove(FGameplayTag Bot);
	virtual bool Add(FGameplayTag Bot);
	
	Flimsy DecoderRing; // use at your own risk, but you might need this in some really narrow cases.
	
protected:
	virtual void CacheLayer();
	
	TCircularBuffer<FTagLayer> CurrentHistory = TCircularBuffer<FTagLayer>(RollbackFrames);
	
	FConservedTagContainer(TSharedPtr<FTagStateRepresentation> Bind, TagsByCode TagsKnown, TagsSeen TagsSeen)
	{
		Tags = Bind;
		DecoderRing = TagsKnown;
		SeenT = TagsSeen;
	}

private:
	TWeakPtr<FConservedTagContainer> AccessRefController;
	TSharedPtr<FTagStateRepresentation> Tags;
	uint64_t CurrentWriteHead = 0;
	TagsSeen SeenT;
};

class ARTILLERYRUNTIME_API AtomicTagArray
{
	friend class UArtilleryDispatch;
	
public:
	AtomicTagArray(const TagsSeen& SeenT) : SeenT(SeenT)
	{
		FastEntities = MakeShareable(new Entities());
	}

	AtomicTagArray();
	bool Add(FSkeletonKey Top, FGameplayTag Bot);
	//lifecycle management for the FConservedTags pointers is out of scope for this class
	//and is instead managed in the client and arbiter, ArtilleryDispatch. I cannot stress this _enough_. Do not try to "fix" this.
	static uint32_t KeyToHash(FSkeletonKey Top);
	bool Find(FSkeletonKey Top, FGameplayTag Bot);
	bool Remove(FSkeletonKey Top, FGameplayTag Bot);
	bool SkeletonKeyExists(FSkeletonKey Top);
	FConservedTags GetReference(FSkeletonKey Top);
	void Init();
	bool Empty();
	
protected:
	//it is STRONGLY advised that you NEVER call EITHER OF THESE directly.
	//please instead use RegisterGameplayTags or DeregisterGameplayTags
	FConservedTags NewTagContainer(FSkeletonKey Top);
	bool Erase(FSkeletonKey Top);
	
private:
	bool AddImpl(uint32_t Key, FGameplayTag Bot);

	unsigned short Counter = 1; //magicify 0.
	TagsSeen SeenT;
	TagsByCode MasterDecoderRing;
	TSharedPtr<Entities> FastEntities;
	constexpr static uint8 STARTED = 1;
	constexpr static uint8 FINISHED = 2;
};
