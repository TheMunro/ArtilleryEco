// Copyright Epic Games, Inc. All Rights Reserved.

#include "ThistleStateTreeConditions.h"

#include "ArtilleryBPLibs.h"
#include "StateTreeExecutionContext.h"
#include "UArtilleryGameplayTagContainer.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(ThistleStateTreeConditions)

#if WITH_EDITOR
#define LOCTEXT_NAMESPACE "StateTreeEditor"

namespace UE::StateTree::Conditions
{
	// ReSharper disable once CppPassValueParameterByConstReference
	//This is a shared pointer. Pass it by ref, and the bloody ref count doesn't go up and you get
	//no bloody benefits to using the shared pointer at all, thank you.
	FText GetArtilleryContainerAsText(const ArtilleryGameplayTagContainerPtr TagContainer, const int ApproxMaxLength = 60)
	{
		FString Combined;
		for (const FGameplayTag& Tag : TagContainer->MyTags)
		{
			FString TagString = Tag.ToString();

			if (Combined.Len() > 0)
			{
				Combined += TEXT(", ");
			}
			
			if (Combined.Len() + TagString.Len() > ApproxMaxLength)
			{
				// Overflow
				if (Combined.Len() == 0)
				{
					Combined += TagString.Left(ApproxMaxLength);
				}
				Combined += TEXT("...");
				break;
			}

			Combined += TagString;
		}

		return FText::FromString(Combined);
	}
	
}

#endif// WITH_EDITOR


//----------------------------------------------------------------------//
//  GameplayTagMatchCondition
//----------------------------------------------------------------------//

bool FArtilleryTagMatchCondition::TestCondition(FStateTreeExecutionContext& Context) const
{
	const FInstanceDataType& InstanceData = Context.GetInstanceData(*this);
	bool found = false;
	auto Container = UArtilleryLibrary::K2_GetTagsByKey(InstanceData.KeyOf, found);
	if (found)
	{
		return (bExactMatch ?  Container->HasTagExact(InstanceData.Tag) : Container->HasTag(InstanceData.Tag)) ^ bInvert;
	}
	return false;
}

bool FArtilleryAttributeValueCondition::Test(float Value, float Target) const
{
		return TreeOperandTest(Value, Target, Operation);
}

bool FArtilleryAttributeValueCondition::TestCondition(FStateTreeExecutionContext& Context) const
{
	const FInstanceDataType& InstanceData = Context.GetInstanceData(*this);
	bool Found = false;
	auto Value = UArtilleryLibrary::implK2_GetAttrib(InstanceData.KeyOf, InstanceData.AttributeName, Found);
	if (Found)
	{
		return Test(Value, TestValue);
	}
	return false;
}

bool FArtilleryAttributeCompareCondition::TestCondition(FStateTreeExecutionContext& Context) const
{
	const FInstanceDataType& InstanceData = Context.GetInstanceData(*this);
	bool Found = false;
	bool TargetFound = false;
	auto Value = UArtilleryLibrary::implK2_GetAttrib(InstanceData.KeyOf, InstanceData.AttributeName, Found);
	auto TestAttrib = UArtilleryLibrary::implK2_GetAttrib(InstanceData.KeyOf, InstanceData.AttributeName, Found);
	if (Found)
	{
		if (TargetFound)
		{
			return Test(Value, TestAttrib);
		}
		else if (bFallbackToTestValue)
		{
			return Test(Value, TestValue);
		}
	}
	return false;
}


bool FArtilleryCompareRelatedCondition::TestCondition(FStateTreeExecutionContext& Context) const
{
	const FInstanceDataType& InstanceData = Context.GetInstanceData(*this);
	bool SourceKey_AttributeFound = false;
	bool RelatedKey_AttributeFound = false;
	bool RelatedKey_FoundAtAll = false;


	auto Identity = UArtilleryLibrary::K2_GetIdentity(InstanceData.KeyOf, Relationship, RelatedKey_FoundAtAll);
	if (RelatedKey_AttributeFound)
	{
		auto TestAttribValue = UArtilleryLibrary::implK2_GetAttrib(Identity, InstanceData.AttributeName, RelatedKey_AttributeFound);
		if (RelatedKey_AttributeFound)
		{
			auto SourceValue = UArtilleryLibrary::implK2_GetAttrib(InstanceData.KeyOf, InstanceData.AttributeName, SourceKey_AttributeFound);
			if (bCompareWithTargetKeyAttribute && SourceKey_AttributeFound)
			{
				return Test(TestAttribValue, SourceValue);
			}
			return Test(TestAttribValue, TestValue);
		}
	}
	return false;
}

bool FArtilleryCompareKeys::TestCondition(FStateTreeExecutionContext& Context) const
{
	const FInstanceDataType& InstanceData = Context.GetInstanceData(*this);
	if (	!InstanceData.SourceKey.IsValid()
		||	!InstanceData.TargetKey.IsValid()
		||	InstanceData.SourceKey != InstanceData.TargetKey)
	{
		return false;
	}
	return true;
}

#if WITH_EDITOR
#undef LOCTEXT_NAMESPACE
#endif // WITH_EDITOR

