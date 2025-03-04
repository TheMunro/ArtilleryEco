#pragma once
#include "SkeletonTypes.h"
#include "Kines.h"
#include "CoreMinimal.h"
#include "InstanceDataTypes.h"
#include "Components/InstancedStaticMeshComponent.h"
#include "SwarmKine.generated.h"

class UObject;

//interface that adds support for owning swarm kines. used for managing many many meshes at a time.
//generally, 
UCLASS()
class SKELETONKEY_API USwarmKineManager : public UInstancedStaticMeshComponent
{
	GENERATED_BODY()
public:
	virtual ~USwarmKineManager() override;
	 typedef int32 IDTYPE;
	 TSharedPtr<TCircularQueue<IDTYPE>> ToRemove;
	 USwarmKineManager()
	 {
	 	PrimaryComponentTick.bCanEverTick = true;
	 	ToRemove = MakeShareable(new TCircularQueue<IDTYPE>(2048));
		KeyToMesh = MakeShareable(new TMap<FSkeletonKey, int32>());
		MeshToKey = MakeShareable(new TMap<int32, FSkeletonKey>());
		KeyToSceneComponent = MakeShareable(new TMap<FSkeletonKey, TObjectPtr<USceneComponent>>());
	 	bDisableCollision = true;
	    UPrimitiveComponent::SetSimulatePhysics(false);
	 	
	}

	// No chaos physics for you
	virtual bool ShouldCreatePhysicsState() const override
	{
		return false;
	}
	
	virtual TOptional<FTransform> GetTransformCopy(FSkeletonKey Target)
	{
		auto m = KeyToMesh->Find(Target);
		FTransform ref;
		if(m)
		{
			if (GetInstanceTransform(GetInstanceIndexForId(FPrimitiveInstanceId(*m)), ref, true))
			{
				return ref;
			}
		}
		return TOptional<FTransform>();
	}
	
	virtual bool SetTransformOnInstance(FSkeletonKey Target, FTransform Update)
	{
		auto m = KeyToMesh->Find(Target);
		if(m)
		{
			auto OptionalLinkedComponent = KeyToSceneComponent->FindRef(Target);
			if (OptionalLinkedComponent && OptionalLinkedComponent.Get())
			{
				OptionalLinkedComponent->SetWorldLocationAndRotationNoPhysics(Update.GetLocation(), Update.Rotator());
			}
			return UpdateInstanceTransform(GetInstanceIndexForId(FPrimitiveInstanceId(*m)), Update, true, false, true);
		}
		return false;
	};
	virtual FSkeletonKey GetKeyOfInstance(FPrimitiveInstanceId Target)
	{
		auto m = MeshToKey->Find(Target.Id);
		return m ? *m : FSkeletonKey();
	};

	virtual void AddToMap(FPrimitiveInstanceId MeshId, FSkeletonKey Key)
	{
		KeyToMesh->Add(Key, MeshId.Id);
		MeshToKey->Add(MeshId.Id, Key);
	}

	void QueueRemoveInstanceById(int I)
	{
		ToRemove->Enqueue(I);
	};

	virtual void CleanupInstance(const FSkeletonKey Target)
	{
		auto HoldOpen = KeyToMesh;
		if (HoldOpen)
		{
			auto m = KeyToMesh->FindRef(Target);
			if (MeshToKey->Contains(m))
			{
				MeshToKey->Remove(m);
				QueueRemoveInstanceById(m);
				KeyToMesh->Remove(Target);
				TObjectPtr<USceneComponent> Out;
				while(KeyToSceneComponent->RemoveAndCopyValue(Target, Out))
				{
					Out->ClearInternalFlags(EInternalObjectFlags::Async);
					Out->ConditionalBeginDestroy();
				}
			}
		}
	}

	virtual TWeakObjectPtr<USceneComponent> GetSceneComponentForInstance(const FSkeletonKey InstanceKey)
	{
		auto m = KeyToSceneComponent->FindRef(InstanceKey);
		if (m && m.Get())
		{
			return m;
		}

		auto NewSceneComponent = TObjectPtr<USceneComponent>(NewObject<USceneComponent>(this)); //async flags are automatically added.
		auto ExistingTransform = GetTransformCopy(InstanceKey);
		if (ExistingTransform.IsSet())
		{
			NewSceneComponent->SetWorldLocationAndRotationNoPhysics(ExistingTransform->GetLocation(), ExistingTransform->Rotator());
		}

		KeyToSceneComponent->Add(InstanceKey, NewSceneComponent);

		return NewSceneComponent;
	}

	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
		FActorComponentTickFunction* ThisTickFunction) override
	{
		Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
		int32 Key = 0;
		while (ToRemove->Dequeue(Key))
		{
			RemoveInstanceById(FPrimitiveInstanceId(Key));
		}
	};
	void BeginDestroy() override;

private:
	TSharedPtr<TMap<FSkeletonKey, int32>> KeyToMesh;
	TSharedPtr<TMap<int32, FSkeletonKey>> MeshToKey;
	TSharedPtr<TMap<FSkeletonKey, TObjectPtr<USceneComponent>>> KeyToSceneComponent;
};

inline USwarmKineManager::~USwarmKineManager()
{


}

inline void USwarmKineManager::BeginDestroy()
{
	Super::BeginDestroy();
	auto hold = KeyToSceneComponent;
	for(auto x : *hold)
	{
		x.Value->ClearInternalFlags(EInternalObjectFlags::Async);
		x.Value->ConditionalBeginDestroy();
	}
	this->ClearInternalFlags(EInternalObjectFlags::Async);
}


class SwarmKine : public Kine
{

	TWeakObjectPtr<USwarmKineManager> MyManager;

public:
	explicit SwarmKine(const TWeakObjectPtr<USwarmKineManager>& MyManager, const FSkeletonKey& MeshInstanceKey)
		: MyManager(MyManager)
	{
		MyKey  = MeshInstanceKey;
	}

	virtual void SetTransformlike(FTransform Input) override
	{
		MyManager->SetTransformOnInstance(MyKey, Input);
	}

	virtual void SetLocationAndRotation(FVector3d Loc, FQuat4d Rot) override
	{
		auto m = MyManager->GetTransformCopy(MyKey);
		if(m.IsSet())
		{
			m->SetLocation(Loc);
			m->SetRotation(Rot);
			MyManager->SetTransformOnInstance(MyKey, m.GetValue());
		}	
	}

	virtual void SetLocationAndRotationWithScope(FVector3d Loc, FQuat4d Rot) override
	{
		auto m = MyManager->GetTransformCopy(MyKey);
		if(m.IsSet())
		{
			m->SetLocation(Loc);
			m->SetRotation(Rot);
			MyManager->SetTransformOnInstance(MyKey, m.GetValue());
		}	
	};

	virtual void SetLocation(FVector3d Location) override
	{
		auto m = MyManager->GetTransformCopy(MyKey);
		if(m.IsSet())
		{
			m->SetLocation(Location);
			MyManager->SetTransformOnInstance(MyKey, m.GetValue());
		}	
	}

	virtual void SetRotation(FQuat4d Rotation) override
	{
		auto m = MyManager->GetTransformCopy(MyKey);
		if(m.IsSet())
		{
			m->SetRotation(Rotation);
			MyManager->SetTransformOnInstance(MyKey, m.GetValue());
		}	
	}
	
protected:
	virtual TOptional<FTransform> CopyOfTransformlike_Impl() override
	{
		return MyManager->GetTransformCopy(MyKey);
	}


};
