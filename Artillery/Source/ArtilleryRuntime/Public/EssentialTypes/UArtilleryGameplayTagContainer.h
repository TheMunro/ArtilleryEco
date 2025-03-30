// Copyright 2024 Oversized Sun Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "CoreTypes.h"

#include "ArtilleryCommonTypes.h"
#include "ArtilleryDispatch.h"
#include "UArtilleryGameplayTagContainer.generated.h"

UENUM(BlueprintType)
/** Whether a tag was added or removed, used in callbacks */
namespace ArtilleryGameplayTagChange {
	enum Type : int
	{		
		/** Event happens when tag is added */
		Added,

		/** Event happens when tag is removed */
		Removed
	};
}

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnArtilleryGameplayTagChanged, const FSkeletonKey, TargetKey, const FGameplayTag, Tag, const ArtilleryGameplayTagChange::Type, TagChangeType);

//ArtilleryGameplayTagContainers serve as a wrapper for the more general borrow-based lifecycle we offer for the
//fgameplaytagcontainer. This assumes ownership and causes the underlying tagcontainer to share its lifecycle.
UCLASS(BlueprintType)
class ARTILLERYRUNTIME_API UArtilleryGameplayTagContainer : public UObject
{
	GENERATED_BODY()
public:

	UPROPERTY(BlueprintReadOnly)
	FSkeletonKey ParentKey;
	TSharedPtr<FGameplayTagContainer> MyTags;
	UArtilleryDispatch* MyDispatch = nullptr;
	bool ReadyToUse = false;
	bool ReferenceOnlyMode = true;
	// Don't use this default constructor, this is a bad
	UArtilleryGameplayTagContainer()
	{
		MyTags = MakeShareable<FGameplayTagContainer>(new FGameplayTagContainer());
	};

	//reference only mode changes the container to act more like you might expect in a blueprint, by not taking ownership of the
	//underlying tag collection. Instead, it will bind to the existing tag collection, if any, and the tag collection will survive
	//the destruction of that container. I've spent a bit of time thinking about this, and it's a pretty despicable hack.
	//On the other hand, it vastly simplifies rollback. On the grasping hand, due to the nature of the key lifecycle, we really need
	//to pick what level construct the tags share their lifecycle with. I don't want to get into a situation where we have a full owner
	//system, either. I think we'll probably end up splitting this into separate ContainerOwner, ContainerRef, and ContainerDisplay
	//classes. That seems the sanest.
	//TODO: revisit NLT 6/8/25
	UArtilleryGameplayTagContainer(FSkeletonKey ParentKeyIn, UArtilleryDispatch* MyDispatchIn, bool ReferenceOnly = false)
	{
		MyTags = MakeShareable<FGameplayTagContainer>(new FGameplayTagContainer());
		Initialize(ParentKeyIn, MyDispatchIn, ReferenceOnly);
	};

	void Initialize(FSkeletonKey ParentKeyIn, UArtilleryDispatch* MyDispatchIn, bool ReferenceOnly = false)
	{
		
		this->ParentKey = ParentKeyIn;
		this->MyDispatch = MyDispatchIn;
		MyTags = MyDispatch->GetGameplayTagContainerAndAddIfNotExists(ParentKeyIn, MyTags);
		ReferenceOnlyMode = ReferenceOnly;
		ReadyToUse = true;
	};

	UFUNCTION(BlueprintCallable, meta = (ScriptName = "ContainerAddTag", DisplayName = "Add tag to container"),  Category="Artillery|Tags")
	void AddTag(const FGameplayTag& TagToAdd)
	{
		// Only add if the tag doesn't already exist
		if (!MyTags->HasTag(TagToAdd))
		{
			MyTags->AddTag(TagToAdd);
		}
	}

	UFUNCTION(BlueprintCallable, meta = (ScriptName = "ContainerRemoveTag", DisplayName = "Remove tag from container"),  Category="Artillery|Tags")
	void RemoveTag(const FGameplayTag& TagToRemove)
	{
		// Only remove if the tag does already exist
		if (MyTags->HasTag(TagToRemove))
		{
			MyTags->RemoveTag(TagToRemove);
		}
	}

	UFUNCTION(BlueprintCallable, meta = (ScriptName = "ContainerHasTag", DisplayName = "Does Container have tag?"),  Category="Artillery|Tags")
	bool HasTag(const FGameplayTag& TagToCheck) const
	{
		return MyTags->HasTag(TagToCheck);
	}

	UFUNCTION(BlueprintCallable, meta = (ScriptName = "ContainerHasTag", DisplayName = "Does Container have tag?"),  Category="Artillery|Tags")
	bool HasTagExact(const FGameplayTag& TagToCheck) const
	{
		return MyTags->HasTagExact(TagToCheck);
	}

	
	UFUNCTION(BlueprintCallable, meta = (ScriptName = "ContainerNumTags", DisplayName = "Number of tags in container"),  Category="Artillery|Tags")
	int32 Num() const
	{
		return MyTags->Num();
	}

	UFUNCTION(BlueprintCallable, meta = (ScriptName = "ContainerMatchesQuery", DisplayName = "Does Container match gameplay tag query?"),  Category="Artillery|Tags")
	bool MatchesQuery(const FGameplayTagQuery& Query) const
	{
		return MyTags->MatchesQuery(Query);
	}

	

	// TODO: expose more of the gameplay tag container's functionality

	~UArtilleryGameplayTagContainer()
	{
		if (MyDispatch && !ReferenceOnlyMode)
		{
			MyDispatch->DeregisterGameplayTags(ParentKey);
		}
	}
};