// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BarrageDispatch.h"
#include "SkeletonTypes.h"
#include "FBarragePrimitive.h"
#include "Components/ActorComponent.h"
#include "BarrageColliderBase.generated.h"

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class UBarrageColliderBase : public UPrimitiveComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UBarrageColliderBase(const FObjectInitializer& ObjectInitializer);
	virtual void InitializeComponent() override;
	FBLet MyBarrageBody = nullptr;
#if UE_ENABLE_DEBUG_DRAWING
	virtual FPrimitiveSceneProxy* CreateSceneProxy() override;
	//if you're seeing stars, you forgot to override this.
	virtual void Draw(FPrimitiveDrawInterface* PDI, const FLinearColor& colour, uint8 DepthPrio)
	{
		//hey don't use the base class or we'll draw weird giant stars on your stuff.
		DrawWireStar(PDI, this->Transform.Location, 100.0, colour, DepthPrio);
		int rotate = GetLastRenderTime();
		rotate %= 10;
		DrawWireStar(PDI, this->Transform.Location + FVector::LeftVector * rotate, 100.0, colour, DepthPrio);
		DrawWireStar(PDI, this->Transform.Location - FVector::LeftVector * rotate, 100.0, colour, DepthPrio);
	}
#endif
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	FSkeletonKey MyObjectKey;
	bool IsReady = false;
	virtual void BeforeBeginPlay(FSkeletonKey TransformOwner);

	//Colliders must override this.
	virtual void Register();

	virtual void OnDestroyPhysicsState() override;
	// Called when the game starts
	virtual void BeginPlay() override;

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;

	void SetTransform(const FTransform& NewTransform);

protected:
	FBTransform Transform;
};

//CONSTRUCTORS
//--------------------
//do not invoke the default constructor unless you have a really good plan. in general, let UE initialize your components.

// Sets default values for this component's properties
inline UBarrageColliderBase::UBarrageColliderBase(const FObjectInitializer& ObjectInitializer) : Super(
	ObjectInitializer)
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.

	PrimaryComponentTick.bCanEverTick = true;

	bWantsInitializeComponent = true;
	MyObjectKey = 0;
	bAlwaysCreatePhysicsState = false;
	UPrimitiveComponent::SetNotifyRigidBodyCollision(false);
	bCanEverAffectNavigation = false;
	Super::SetCollisionEnabled(ECollisionEnabled::Type::NoCollision);
	Super::SetEnableGravity(false);
	Super::SetSimulatePhysics(false);
}

//---------------------------------

inline void UBarrageColliderBase::InitializeComponent()
{
	Super::InitializeComponent();
}

//SETTER: Unused example of how you might set up a registration for an arbitrary key.
inline void UBarrageColliderBase::BeforeBeginPlay(FSkeletonKey TransformOwner)
{
	MyObjectKey = TransformOwner;
}

//Colliders must override this.
inline void UBarrageColliderBase::Register()
{
	PrimaryComponentTick.SetTickFunctionEnable(false);
}

inline void UBarrageColliderBase::TickComponent(float DeltaTime, ELevelTick TickType,
                                                FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	Register(); // ...
}

// Called when the game starts
inline void UBarrageColliderBase::BeginPlay()
{
	Super::BeginPlay();
	Register();
}

//TOMBSTONERS

inline void UBarrageColliderBase::OnDestroyPhysicsState()
{
	Super::OnDestroyPhysicsState();
	if (GetWorld())
	{
		auto Physics = GetWorld()->GetSubsystem<UBarrageDispatch>();
		if (Physics && MyBarrageBody)
		{
			Physics->SuggestTombstone(MyBarrageBody);
			MyBarrageBody.Reset();
		}
	}
}

inline FPrimitiveSceneProxy* UBarrageColliderBase::CreateSceneProxy()
{
	class FBarrageSceneProxy : public FPrimitiveSceneProxy
	{
	public:
		TWeakObjectPtr<UBarrageColliderBase> ProxyOf;

		explicit FBarrageSceneProxy(UBarrageColliderBase* BarrageColliderBase): FPrimitiveSceneProxy(
			BarrageColliderBase)
		{
			ProxyOf = MakeWeakObjectPtr(BarrageColliderBase);
			bWillEverBeLit = false;
		}

		virtual void GetDynamicMeshElements(const TArray<const FSceneView*>& Views, const FSceneViewFamily& ViewFamily,
		                                    uint32 VisibilityMap, FMeshElementCollector& Collector) const override
		{
			if (!ProxyOf.IsValid())
			{
				return;
			}

			if (IsSelected())
			{
				return;
			}


			for (int32 Vind = 0; Vind < Views.Num(); Vind++)
			{
				const FSceneView* View = Views[Vind];

				FPrimitiveDrawInterface* PDI = Collector.GetPDI(Vind);

				FLinearColor Draw = GetViewSelectionColor(
					FLinearColor::Yellow,
					*View,
					IsSelected(),
					IsHovered(),
					false,
					IsIndividuallySelected());

				ProxyOf.Get()->Draw(PDI, Draw, SDPG_World);
			}
		}

		virtual uint32 GetMemoryFootprint() const override
		{
			return sizeof *this; // no. dynamic. data. thanks.
		}

		virtual SIZE_T GetTypeHash() const override
		{
			return PointerHash(this);
		}
	};
	return new FBarrageSceneProxy(this);
}

inline void UBarrageColliderBase::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	if (GetWorld())
	{
		auto Physics = GetWorld()->GetSubsystem<UBarrageDispatch>();
		if (Physics && MyBarrageBody)
		{
			Physics->SuggestTombstone(MyBarrageBody);
			MyBarrageBody.Reset();
		}
	}
}

inline void UBarrageColliderBase::SetTransform(const FTransform& NewTransform)
{
	Transform.SetTransform(NewTransform);
}
