// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BarrageColliderBase.h"
#include "BarrageDispatch.h"
#include "SkeletonTypes.h"
#include "KeyCarry.h"
#include "Components/ActorComponent.h"
#include "BarrageStaticAutoMesh.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ARTILLERYRUNTIME_API UBarrageStaticAutoMesh : public UBarrageColliderBase
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UBarrageStaticAutoMesh(const FObjectInitializer& ObjectInitializer);
	virtual void Register() override;
#if UE_ENABLE_DEBUG_DRAWING
	virtual void Draw(FPrimitiveDrawInterface* PDI, const FLinearColor& colour, uint8 DepthPrio) override
	{
		AActor* Actor = GetOwner();
		if (Actor)
		{
			UStaticMeshComponent* MeshPtr = Actor->GetComponentByClass<UStaticMeshComponent>();
			if(MeshPtr)
			{
				FBox CompanionRectangle = MeshPtr->GetNavigationBounds();//since we actually use the collision, this might work sometimes!
				CompanionRectangle = CompanionRectangle.IsValid && CompanionRectangle.GetVolume() >= 1.0? CompanionRectangle :
				FBox(MeshPtr->GetComponentLocation() -10,MeshPtr->GetComponentLocation() + 10); 
				DrawWireBox(PDI, CompanionRectangle, colour, DepthPrio);
			}
		}
	}
#endif
};

//CONSTRUCTORS
//--------------------

// Sets default values for this component's properties
inline UBarrageStaticAutoMesh::UBarrageStaticAutoMesh(const FObjectInitializer& ObjectInitializer) : Super(
	ObjectInitializer)
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	
	bWantsInitializeComponent = true;
	PrimaryComponentTick.bCanEverTick = true;
	MyObjectKey = 0;
}

inline void UBarrageStaticAutoMesh::Register()
{
	if(MyObjectKey ==0 )
	{
		if(GetOwner())
		{
			if(GetOwner()->GetComponentByClass<UKeyCarry>())
			{
				MyObjectKey = GetOwner()->GetComponentByClass<UKeyCarry>()->GetMyKey();
			}
			
			if(MyObjectKey == 0)
			{
				auto val = PointerHash(GetOwner());
				ActorKey TopLevelActorKey = ActorKey(val);
				MyObjectKey = TopLevelActorKey;
			}
		}
	}
	if(!IsReady && MyObjectKey != 0 && GetOwner()) // this could easily be just the !=, but it's better to have the whole idiom in the example
	{
		UBarrageDispatch* Physics =  GetWorld()->GetSubsystem<UBarrageDispatch>();
		AActor* Actor = GetOwner();
		SetTransform(Actor->GetActorTransform());
		
		UStaticMeshComponent* MeshPtr = Actor->GetComponentByClass<UStaticMeshComponent>();
		if(MeshPtr)
		{
			// remember, jolt coords are X,Z,Y. BUT we don't want to scale the scale. this breaks our coord guidelines
			// by storing the jolted ver in the params but oh well.
			MyBarrageBody = Physics->LoadComplexStaticMesh(Transform, MeshPtr, MyObjectKey);
		}
		if(MyBarrageBody)
		{
			IsReady = true;
		}
	}
	if(IsReady)
	{
		PrimaryComponentTick.SetTickFunctionEnable(false);
	}
}