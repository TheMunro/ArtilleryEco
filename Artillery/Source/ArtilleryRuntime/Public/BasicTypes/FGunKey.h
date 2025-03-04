// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "skeletonize.h"
#include "SkeletonTypes.h"
#include "CoreMinimal.h"
#include "CoreTypes.h"
#include "Templates/SubclassOf.h"
#include "UObject/UnrealType.h"
#include "Engine/DataTable.h"

#include "Containers/CircularBuffer.h"
#include "FGunKey.generated.h"


USTRUCT(BlueprintType)
struct ARTILLERYRUNTIME_API FGunKey
{
	GENERATED_BODY()

	
public:
	FGunKey():
	// ReSharper disable once CppRedundantMemberInitializer
	// THIS DOES NOT DO THE SAME THING AS INSTANCE 0
	GunDefinitionID("M6D"), GunInstanceID()
	{
		
	}
	FGunKey(const FString& Name):
	
// ReSharper disable once CppRedundantMemberInitializer
// THIS DOES NOT DO THE SAME THING AS INSTANCE 0
GunDefinitionID(Name), GunInstanceID()
	{
		
	}
	explicit FGunKey(const FString& Name, FGunInstanceKey id):
	GunDefinitionID(Name), GunInstanceID(id)
	{
	}
	FGunKey(const FString& Name, uint32_t id):
	GunDefinitionID(Name), GunInstanceID(id)
	{
	}
	UPROPERTY(BlueprintReadOnly)
	FString GunDefinitionID; //this will need to be human searchable
	//FUN STORY: BLUEPRINT CAN'T USE UINT64.
	FGunInstanceKey GunInstanceID;
	
	operator FSkeletonKey() const
	{
		return FSkeletonKey(FORGE_SKELETON_KEY(GetTypeHash(GunDefinitionID) + GetTypeHash(GunInstanceID), SKELLY::SFIX_ART_GUNS));
	}
	friend uint32 GetTypeHash(const FGunKey& Other)
	{
		// it's probably fine!
		return GetTypeHash(Other.GunDefinitionID) + GetTypeHash(Other.GunInstanceID);
	}

	bool IsValidInstance() const { return GunInstanceID != 0; }
};
static bool operator==(FGunKey const& lhs, FGunKey const& rhs) {
	return (lhs.GunDefinitionID == rhs.GunDefinitionID) && (lhs.GunInstanceID == rhs.GunInstanceID);
}
//when sorted, gunkeys follow their instantiation order!
static bool operator<(FGunKey const& lhs, FGunKey const& rhs) {
	return (lhs.GunInstanceID < rhs.GunInstanceID);
}
//see you soon, chief...
static const FGunKey DefaultGunKey = FGunKey("M6D");