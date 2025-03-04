// Copyright 2024 Oversized Sun Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "FWorldSimOwner.h"
#include "ArtilleryDispatch.h"
#include "GameFramework/Actor.h"
#include "SwarmKine.h"
#include "EPhysicsLayer.h"
#include "AInstancedMeshManager.generated.h"

UCLASS()
class ARTILLERYRUNTIME_API AInstancedMeshManager : public AActor
{
	GENERATED_BODY()
public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Artillery, meta = (AllowPrivateAccess = "true"))
	USwarmKineManager* SwarmKineManager;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Artillery, meta = (AllowPrivateAccess = "true"))
	UArtilleryDispatch* MyDispatch;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Artillery, meta = (AllowPrivateAccess = "true"))
	UTransformDispatch* TransformDispatch;

	uint32 instances_generated;
	
	virtual void BeginPlay() override
	{
		Super::BeginPlay();

		if (!Usable)
		{
			InitializeManager();
		}
	}

public:
	bool Usable = false;
	
	AInstancedMeshManager()
	{
		SwarmKineManager = CreateDefaultSubobject<USwarmKineManager>("SwarmKineManager");
		SwarmKineManager->bDisableCollision = true;
		SwarmKineManager->SetSimulatePhysics(false);
		SetActorEnableCollision(false);
		DisableComponentsSimulatePhysics();
		MyDispatch = nullptr;
		TransformDispatch = nullptr;
	}

	virtual void BeginDestroy() override
	{
		Super::BeginDestroy();// explicitly begin the destruction of the swarmkine manager.
		//one super fun thing is that because all the pointers are weak refs, nothing gets automatically collected
		if(SwarmKineManager)
		{
			SwarmKineManager->ClearInternalFlags(EInternalObjectFlags::Async);
			SwarmKineManager->ConditionalBeginDestroy();	
		}
	}

	ActorKey GetMyKey() const
	{
		return MyKey;
	};

	void SetStaticMesh(UStaticMesh* Mesh)
	{
		SwarmKineManager->SetStaticMesh(Mesh);
	}

	void InitializeManager()
	{
		if(!Usable)
		{
			MyDispatch = GetWorld()->GetSubsystem<UArtilleryDispatch>();
			TransformDispatch = GetWorld()->GetSubsystem<UTransformDispatch>();
			// No Chaos for you!
			SwarmKineManager->SetEnableGravity(false);
			SwarmKineManager->SetSimulatePhysics(false);
			SwarmKineManager->DestroyPhysicsState();

			// Make a key yo
			auto keyHash = PointerHash(this);
			UE_LOG(LogTemp, Warning, TEXT("AInstancedMeshManager Parented: %d"), keyHash);
			MyKey = ActorKey(keyHash);
			Usable = true;
		}
	}
	
	FSkeletonKey GenerateNewProjectileKey()
	{
		return FSkeletonKey(HashCombine(GetTypeHash(SwarmKineManager), GetTypeHash(++instances_generated)));
	}

	UFUNCTION(BlueprintCallable, Category = Instance)
	FSkeletonKey CreateNewInstance(const FTransform& WorldTransform, const FVector& MuzzleVelocity, const EPhysicsLayer Layer, float Scale = 1.0f, bool IsSensor = false, bool IsDynamic = false)
	{
		return CreateNewInstance(WorldTransform, MuzzleVelocity, static_cast<uint16_t>(Layer), Scale, FSkeletonKey(), IsSensor, IsDynamic);
	}
	
	FSkeletonKey CreateNewInstance(const FTransform& WorldTransform, const FVector3d& MuzzleVelocity, const uint16_t Layer, float Scale = 1.0f, const FSkeletonKey& ExistingKey = FSkeletonKey::Invalid(), bool IsSensor = false, bool IsDynamic = false)
	{
		// TODO: Does this make a good hash? Can we hash collide?
		// TODO: Oh god this definitely birthday problems at some point but I don't know how else to get a unique hash since the instances rotate around and reuse the same memory
		FSkeletonKey NewInstanceKey = ExistingKey == FSkeletonKey::Invalid() ? GenerateNewProjectileKey() : ExistingKey;

		auto ScaledTransform = FTransform(FRotator::ZeroRotator,WorldTransform.GetLocation(), FVector3d(Scale, Scale, Scale));
		FPrimitiveInstanceId NewInstanceId = SwarmKineManager->AddInstanceById(ScaledTransform, true);
		SwarmKineManager->AddToMap(NewInstanceId, NewInstanceKey);

		CreateNewInstanceWithKeyInternal(NewInstanceKey, WorldTransform, MuzzleVelocity, Layer, Scale);
		
		return NewInstanceKey;
	}

	//TODO: this really really really should return a fblet or a kine OR make it impossible to get a FBlet or kine for that scene component.
	//we do not ever want scene components that are managed in two or more ways.
	TWeakObjectPtr<USceneComponent> GetSceneComponentForInstance(const FSkeletonKey InstanceKey)
	{
		return SwarmKineManager->GetSceneComponentForInstance(InstanceKey);
	}

	// THIS MUST BE CALLED OR ELSE THE MAPPINGS WILL KEEP THE LIVE REFERENCE 4EVA

	void CleanupInstance(const FSkeletonKey Target)
	{
		auto Physics = GetWorld()->GetSubsystem<UBarrageDispatch>();
		Physics->SuggestTombstone(Physics->GetShapeRef(Target));
		TransformDispatch->ReleaseKineByKey(Target);
		SwarmKineManager->CleanupInstance(Target);
	}

private:
	void CreateNewInstanceWithKeyInternal(const FSkeletonKey& ProjectileKey, const FTransform& WorldTransform, const FVector3d& MuzzleVelocity, const uint16_t Layer, float Scale) const
	{
		// TODO: can't use the BarrageColliderBase set of types, so in-lining the barrage setup code. Is this what we want long-term?
		auto Physics = GetWorld()->GetSubsystem<UBarrageDispatch>();
		auto AnyMesh = SwarmKineManager->GetStaticMesh();
		auto Boxen = AnyMesh->GetBoundingBox();
		auto extents = Boxen.GetExtent() * 2 * Scale;

		auto params = FBarrageBounder::GenerateBoxBounds(WorldTransform.GetLocation(), extents.X, extents.Y, extents.Z,
			FVector3d(0, 0, extents.Z/2));
		
		FBLet MyBarrageBody = Physics->CreateProjectile(params, ProjectileKey, Layer);
		TransformDispatch->RegisterObjectToShadowTransform(ProjectileKey, SwarmKineManager);
		FBarragePrimitive::SetVelocity(MuzzleVelocity, MyBarrageBody);

		FBarragePrimitive::SetGravityFactor(0.f, MyBarrageBody);
		
		MyDispatch->REGISTER_PROJECTILE_FINAL_TICK_RESOLVER(100, ProjectileKey);
	}
	
	ActorKey MyKey;
};