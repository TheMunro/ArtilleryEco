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

UCLASS(BlueprintType)
class ARTILLERYRUNTIME_API UArtilleryGameplayTagContainer : public UObject
{
	GENERATED_BODY()
public:

	UPROPERTY(BlueprintReadOnly)
	FSkeletonKey ParentKey;
	FGameplayTagContainer MyTags;
	UArtilleryDispatch* MyDispatch = nullptr;
	bool ReadyToUse = false;

	// Don't use this default constructor, this is a bad
	UArtilleryGameplayTagContainer()
	{
		
	};

	UArtilleryGameplayTagContainer(FSkeletonKey ParentKeyIn, UArtilleryDispatch* MyDispatchIn)
	{
		Initialize(ParentKeyIn, MyDispatchIn);
	};

	void Initialize(FSkeletonKey ParentKeyIn, UArtilleryDispatch* MyDispatchIn)
	{
		this->ParentKey = ParentKeyIn;
		this->MyDispatch = MyDispatchIn;

		MyDispatch->RegisterGameplayTags(ParentKeyIn, this);

		ReadyToUse = true;
	};

	UFUNCTION(BlueprintCallable, meta = (ScriptName = "ContainerAddTag", DisplayName = "Add tag to container"),  Category="Artillery|Tags")
	void AddTag(const FGameplayTag& TagToAdd)
	{
		// Only add if the tag doesn't already exist
		if (!MyTags.HasTag(TagToAdd))
		{
			MyTags.AddTag(TagToAdd);
		}
	}

	UFUNCTION(BlueprintCallable, meta = (ScriptName = "ContainerRemoveTag", DisplayName = "Remove tag from container"),  Category="Artillery|Tags")
	void RemoveTag(const FGameplayTag& TagToRemove)
	{
		// Only remove if the tag does already exist
		if (MyTags.HasTag(TagToRemove))
		{
			MyTags.RemoveTag(TagToRemove);
		}
	}

	UFUNCTION(BlueprintCallable, meta = (ScriptName = "ContainerHasTag", DisplayName = "Does Container have tag?"),  Category="Artillery|Tags")
	bool HasTag(const FGameplayTag& TagToCheck) const
	{
		return MyTags.HasTag(TagToCheck);
	}

	UFUNCTION(BlueprintCallable, meta = (ScriptName = "ContainerHasTag", DisplayName = "Does Container have tag?"),  Category="Artillery|Tags")
	bool HasTagExact(const FGameplayTag& TagToCheck) const
	{
		return MyTags.HasTagExact(TagToCheck);
	}

	UFUNCTION(BlueprintCallable, meta = (ScriptName = "ContainerHasAny", DisplayName = "Does Container share any tags with other container?"),  Category="Artillery|Tags")
	bool HasAny(const UArtilleryGameplayTagContainer* OtherContainer) const
	{
		return MyTags.HasAny(OtherContainer->MyTags);
	}

	UFUNCTION(BlueprintCallable, meta = (ScriptName = "ContainerHasAll", DisplayName = "Does Container have all tags of other container?"),  Category="Artillery|Tags")
	bool HasAll(const UArtilleryGameplayTagContainer* OtherContainer) const
	{
		return MyTags.HasAll(OtherContainer->MyTags);
	}

	bool HasAll(const FGameplayTagContainer& OtherContainer) const
	{
		return MyTags.HasAll(OtherContainer);
	}

	UFUNCTION(BlueprintCallable, meta = (ScriptName = "ContainerNumTags", DisplayName = "Number of tags in container"),  Category="Artillery|Tags")
	int32 Num() const
	{
		return MyTags.Num();
	}

	UFUNCTION(BlueprintCallable, meta = (ScriptName = "ContainerMatchesQuery", DisplayName = "Does Container match gameplay tag query?"),  Category="Artillery|Tags")
	bool MatchesQuery(const FGameplayTagQuery& Query) const
	{
		return MyTags.MatchesQuery(Query);
	}

	// TODO: expose more of the gameplay tag container's functionality

	~UArtilleryGameplayTagContainer()
	{
		if (MyDispatch)
		{
			MyDispatch->DeregisterGameplayTags(ParentKey);
		}
	}
};