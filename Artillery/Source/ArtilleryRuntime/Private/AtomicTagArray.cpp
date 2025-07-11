﻿#include "AtomicTagArray.h"

bool FTagStateRepresentation::Find(uint16 Numerology)
{
	for (uint8 i = 0; i < FAST_TAG_MAX_C; i++)
	{
		if (Tags[i] == Numerology)
		{
			return true;
		}
	}
	return false;
}

bool FTagStateRepresentation::Remove(uint16 Numerology)
{
	bool foundandremoved = false;
	for (uint8 i = 0; i < FAST_TAG_MAX_C; i++)
	{
		if (Tags[i] == Numerology)
		{
			Tags[i] = 0;
			[[maybe_unused]] uint32_t A = snagged.fetch_and(0 << i, std::memory_order_acquire);
			foundandremoved = true;
		}
	}
	return foundandremoved;
}

bool FTagStateRepresentation::Add(uint16 Numerology)
{
	for (uint8 i = 0; i < FAST_TAG_MAX_C; i++)
	{
		//attempt weakly to prevent double entry. remove removes all instances, so this isn't a huge problem
		//we just want to make it only arise when contested in specific ways.
		if (Tags[i] == Numerology)
			return true;
	}
	for (uint8 i = 0; i < FAST_TAG_MAX_C; i++)
	{
		//this protects us from add races pretty well. now to get a double entry we need a fairly specific sequence
		//to all line up to contend.
		if (Tags[i] == 0 || Tags[i] == Numerology)
		{
			uint32_t A = snagged.fetch_and(1 << i, std::memory_order_acquire);
			if ((A & 1 << i) == 0) //was unset, is now set.
			{
				//we got it.
				Tags[i] = Numerology;
				return true;
			}
		}
	}

	return false;
}

void FConservedTagContainer::CacheLayer()
{
	uint32 index = CurrentHistory.GetNextIndex(CurrentWriteHead);
	CurrentHistory[index] = FTagLayer();
	TSharedPtr<UnderlyingTagReverse> WornRing = DecoderRing.Pin();
	if (WornRing)
	{
		for (uint16_t tagcode : Tags->Tags)
		{
			FGameplayTag* ATag = WornRing->Find(tagcode);
			if (ATag != nullptr)
			{
				CurrentHistory[index]->Add(*ATag);
			}
		}
		++CurrentWriteHead;
	}
}

FConservedTags FConservedTagContainer::GetReference()
{
	if (AccessRefController.IsValid())
	{
		TSharedPtr<FConservedTagContainer> scopeguard = AccessRefController.Pin();
		return scopeguard;
	}
	return nullptr;
}

//frame numbering starts at _1_
FTagLayer FConservedTagContainer::GetFrameByNumber(uint64_t FrameNumber)
{
	return CurrentHistory[CurrentHistory.GetNextIndex(FrameNumber-1)];
}

bool FConservedTagContainer::Find(FGameplayTag Bot)
{
	if (Tags)
	{
		if (uint16_t* search = SeenT->Find(Bot); search != nullptr)
		{
			uint16_t Numerology = *search;
			return Tags->Find(Numerology);
		}
	}
	return false;
}

bool FConservedTagContainer::Remove(FGameplayTag Bot)
{
	if (Tags)
	{
		if (uint16_t* search = SeenT->Find(Bot); search != nullptr)
		{
			uint16_t Numerology = *search;
			return Tags->Remove(Numerology);
		}
	}
	return false;
}

bool FConservedTagContainer::Add(FGameplayTag Bot)
{
	if (Tags)
	{
		if (uint16_t* search = SeenT->Find(Bot); search != nullptr)
		{
			uint16_t Numerology = *search;
			return Tags->Add(Numerology);
		}
	}
	return false;
}

//todo: doublecheck my math here.
TSharedPtr<TArray<FGameplayTag>> FConservedTagContainer::GetAllTags()
{
	return CurrentHistory[CurrentHistory.GetPreviousIndex(CurrentWriteHead)];
}

//todo: again doublecheck my math here. can peek the FConservedAttrib. ATM, I gotta get this wired up.
TSharedPtr<TArray<FGameplayTag>> FConservedTagContainer::GetAllTags(uint64_t FrameNumber)
{
	return CurrentHistory[CurrentHistory.GetPreviousIndex(FrameNumber)];
}

bool RecordTags()
{
	return true;
}

AtomicTagArray::AtomicTagArray()
{
}

void AtomicTagArray::Init()
{
	SeenT = MakeShareable(new UnderlyingTagMapping());
	MasterDecoderRing = MakeShareable(new UnderlyingTagReverse());
	FastEntities = MakeShareable(new Entities());
	FGameplayTagContainer Container;
	UGameplayTagsManager::Get().RequestAllGameplayTags(Container, false);
	TArray<FGameplayTag> TagArray;
	Container.GetGameplayTagArray(TagArray);
	for (FGameplayTag& Tag : TagArray)
	{
		SeenT->Emplace(Tag, ++Counter);
		MasterDecoderRing->Emplace(Counter,Tag);
	}
}

bool AtomicTagArray::Add(FSkeletonKey Top, FGameplayTag Bot)
{
	uint32_t Key = KeyToHash(Top);
	if (!AddImpl(Key, Bot))
	{
		return false; // we really should do more but right now, this is just for entities with seven tags or less.
		//I had a sketch of something that was a little more versatile, but ultimately, I actually think it might be
		//overkill. entities should EITHER get added to FastSKRP or to the normal tag set.
	}
	return true;
	//okay this is a satanic mess.
}

//it is STRONGLY advised that you NEVER call this directly.
//please instead use RegisterGameplayTags or DeregisterGameplayTags
FConservedTags AtomicTagArray::NewTagContainer(FSkeletonKey Top)
{
	uint32_t Key = KeyToHash(Top);
	TSharedPtr<Entities> HOpen = FastEntities;
	if (FTagsPtr Tags; HOpen && !HOpen->find(Key, Tags))
	{
		Tags = MakeShareable<FTagStateRepresentation>(new FTagStateRepresentation());
		FConservedTags That = MakeShareable(new FConservedTagContainer(Tags, MasterDecoderRing, SeenT));
		That->AccessRefController =  That.ToWeakPtr();
		Tags->AccessRefController = That->AccessRefController;
		FastEntities->insert_or_assign(Key, Tags);
		return That; //this begins a chain of events that leads to everything getting reference counted correctly.
	}
	return nullptr; // this is a lot tidier but it's still a horror.
}

uint32_t AtomicTagArray::KeyToHash(FSkeletonKey Top)
{
	uint64_t TypeInfo = GET_SK_TYPE(Top);
	uint64_t KeyInstance = Top & SKELLY::SFIX_HASH_EXT;
	return HashCombineFast(TypeInfo, KeyInstance);
}

bool AtomicTagArray::Find(FSkeletonKey Top, FGameplayTag Bot)
{
	uint32_t Key = KeyToHash(Top);
	TSharedPtr<Entities> HOpen = FastEntities;
	if (uint16_t* search = SeenT->Find(Bot); HOpen && search != nullptr)
	{
		uint16_t Numerology = *search;
		FTagsPtr Tags;
		return !HOpen->find(Key, Tags) ? false : Tags->Find(Numerology);
	}
	return false;
}

bool AtomicTagArray::Erase(FSkeletonKey Top)
{
	uint32_t Key = KeyToHash(Top);
	TSharedPtr<Entities> HOpen = FastEntities;
	return HOpen ? FastEntities->erase(Key) : true;
}

// ReSharper disable once CppMemberFunctionMayBeConst
//returns true if the tag is not a real tag OR if the owning entity was removed OR the tag was removed 
//returns false if tag and entity are live but no tag was removed.
bool AtomicTagArray::Remove(FSkeletonKey Top, FGameplayTag Bot)
{
	uint32_t Key = KeyToHash(Top);
	TSharedPtr<Entities> HOpen = FastEntities;
	if (uint16_t* search = SeenT->Find(Bot); search != nullptr)
	{
		FTagsPtr Tags;
		uint16_t Numerology = *search;
		if (HOpen && !HOpen->find(Key, Tags))
		{
			return true;
		}
		Tags->Remove(Numerology);
	}
	return false;
}

bool AtomicTagArray::SkeletonKeyExists(FSkeletonKey Top)
{
	uint32_t Key = KeyToHash(Top);
	TSharedPtr<Entities> HOpen = FastEntities;
	return HOpen ? HOpen->contains(Key) : false;
}

inline FConservedTags AtomicTagArray::GetReference(FSkeletonKey Top)
{
	FTagsPtr into = nullptr;
	uint32_t Key = KeyToHash(Top);
	TSharedPtr<Entities> HOpen = FastEntities;
	return HOpen && HOpen->find(Key, into) && into.IsValid() ? into->AccessRefController.Pin() : FConservedTags();
}

bool AtomicTagArray::Empty()
{
	TSharedPtr<Entities> HOpen = FastEntities;
	FastEntities.Reset();
	SeenT.Reset(); // let it go, but don't blast it.
	MasterDecoderRing.Reset();
	return true;
}

bool AtomicTagArray::AddImpl(uint32_t Key, FGameplayTag Bot)
{
	TSharedPtr<Entities> HOpen = FastEntities;
	if (uint16_t* search = SeenT->Find(Bot); HOpen && search != nullptr)
	{
		FTagsPtr Tags;
		uint16_t Numerology = *search;
		return !HOpen->find(Key, Tags) ? false : Tags->Add(Numerology);
	}
	return false;
}
