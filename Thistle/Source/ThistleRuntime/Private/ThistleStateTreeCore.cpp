#include "ThistleStateTreeCore.h"
#include "StateTreeConditionBase.h"
#include "ThistleBehavioralist.h"
#include "ThistleStateTreeConditions.h"
#include "ThistleEvaluators.h"
#include "ThistleDispatch.h"
#include "MassStateTreeSchema.h"
#include "Components/StateTreeComponentSchema.h"
#include "Public/GameplayTags.h"

EStateTreeRunStatus FStoreRelationship::Tick(FStateTreeExecutionContext& Context,
                                             const float DeltaTime) const
{
	const FInstanceDataType& InstanceData = Context.GetInstanceData(*this);

	if (const IdentPtr V = UArtilleryDispatch::SelfPtr->
		GetOrAddIdent(InstanceData.SourceKey, InstanceData.Relationship))
	{
		V->SetCurrentValue(InstanceData.UpdateToRelatedKey);
		return EStateTreeRunStatus::Succeeded;
	}
	return EStateTreeRunStatus::Running;
}

EStateTreeRunStatus FSetTagOfKey::Tick(FStateTreeExecutionContext& Context, const float DeltaTime) const
{
	const FInstanceDataType& InstanceData = Context.GetInstanceData(*this);

	//This seems unsafe.
	UArtilleryDispatch::SelfPtr->AddTagToEntity(InstanceData.KeyOf, InstanceData.Tag);
	return EStateTreeRunStatus::Succeeded;
}

EStateTreeRunStatus FRemoveTagFromKey::Tick(FStateTreeExecutionContext& Context, const float DeltaTime) const
{
	const FInstanceDataType& InstanceData = Context.GetInstanceData(*this);

	//This seems unsafe.
	UArtilleryDispatch::SelfPtr->RemoveTagFromEntity(InstanceData.KeyOf, InstanceData.Tag);
	return EStateTreeRunStatus::Succeeded;
}

EStateTreeRunStatus FStoreToAttribute::Tick(FStateTreeExecutionContext& Context, const float DeltaTime) const
{
	const FInstanceDataType& InstanceData = Context.GetInstanceData(*this);

	auto V = UArtilleryDispatch::SelfPtr->GetAttrib(InstanceData.KeyOf, InstanceData.AttributeName);
	if (V)
	{
		V->SetCurrentValue(InstanceData.Value);
		return EStateTreeRunStatus::Succeeded;
	}
	return EStateTreeRunStatus::Running;
}

////////////////////////MOVE TO

void FMoveOrder::ExitState(FStateTreeExecutionContext& Context,
                                      const FStateTreeTransitionResult& Transition) const
{
}

void FMoveOrder::StateCompleted(FStateTreeExecutionContext& Context,
                                           const EStateTreeRunStatus CompletionStatus,
                                           const FStateTreeActiveStates& CompletedActiveStates) const
{
}

EStateTreeRunStatus FMoveOrder::AttemptMovePath(FStateTreeExecutionContext& Context, FVector location,
                                                           FVector HereIAm) const
{
	const FInstanceDataType& InstanceData = Context.GetInstanceData(*this);
	auto AreWeBarraging = UBarrageDispatch::SelfPtr;

	if (AreWeBarraging != nullptr && UThistleBehavioralist::SelfPtr)
	{
		bool found = false;
		auto tagc = UArtilleryLibrary::K2_GetTagsByKey(InstanceData.KeyOf, found);
		if (found)
		{
			tagc->RemoveTag(TAG_Orders_Move_Needed);
		}

		UThistleBehavioralist::SelfPtr->BounceTag(InstanceData.KeyOf, TAG_Orders_Move_Needed,
		                                          UThistleBehavioralist::DelayBetweenMoveOrders); 

		if ((HereIAm - location).Length() < FMath::Max(0.01f, Tolerance))
		{
			if (found)
			{
				tagc->AddTag(TAG_Orders_Move_Needed);
			}
			return EStateTreeRunStatus::Succeeded;
		}

		if (!UThistleBehavioralist::AttemptInvokePathingOnKey(InstanceData.KeyOf, location))
		{
			if (found)
			{
				tagc->AddTag(TAG_Orders_Move_Needed);
			}
			return EStateTreeRunStatus::Failed;
		}
		// ReSharper disable once CppRedundantElseKeywordInsideCompoundStatement
		else
		{
			return EStateTreeRunStatus::Succeeded;
		}
	}
	return EStateTreeRunStatus::Failed;
}

EStateTreeRunStatus FMoveOrder::Tick(FStateTreeExecutionContext& Context, const float DeltaTime) const
{
	const FInstanceDataType& InstanceData = Context.GetInstanceData(*this);
	bool Shuck = false;
	auto location = InstanceData.ShuckPoi(Shuck);
	if (!Shuck)
	{
		return EStateTreeRunStatus::Failed;
	}
	//run on cadence.
	if (UArtilleryLibrary::GetTotalsTickCount() % 64)
	{
		bool found = false;
		auto HereIAm = UArtilleryLibrary::implK2_GetLocation(InstanceData.KeyOf, found);
		if (found && (HereIAm - location).Length() > Tolerance)
		{
			return AttemptMovePath(Context, location, HereIAm);
		}
	}
	return EStateTreeRunStatus::Running;
}

void FMoveOrder::TriggerTransitions(FStateTreeExecutionContext& Context) const
{
}

void UThistleStateTreeLease::BeginDestroy()
{
	
	IsReady = false;
	bIsRunning = false;
	Super::BeginDestroy();
}

void UThistleStateTreeLease::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	IsReady = false;
	bIsRunning = false;
	
	Super::EndPlay(EndPlayReason);
}

void UThistleStateTreeLease::OnUnregister()
{
	
	IsReady = false;
	bIsRunning = false;
	Super::OnUnregister();
}

FString UThistleStateTreeLease::GetDebugInfoString() const
{
	if (this && GetOwner() && StateTreeRef.IsValid())
	{
		if (!StateTreeRef.IsValid())
		{
			return FString("No StateTree to run.");
		}
		return FConstStateTreeExecutionContextView(*GetOwner(), *StateTreeRef.GetStateTree(), InstanceData).Get().
			GetDebugInfoString();
	}
	else
	{
		return FString("No StateTree to run.");
	}
}

void UThistleStateTreeLease::InitializeComponent()
{
	Super::InitializeComponent();
}

bool UThistleStateTreeLease::CollectExternalData(const FStateTreeExecutionContext& Context, const UStateTree* StateTree,
                                                 TArrayView<const FStateTreeExternalDataDesc> Descs,
                                                 TArrayView<FStateTreeDataView> OutDataViews) const
{
	auto ErrantWays = const_cast<F_ArtilleryKeyInstanceData*>(&InstanceOwnerKey);
	bool First = UThistleStateTreeSchema::CollectExternalData(Context, ErrantWays, Descs, OutDataViews);
	return First && Super::CollectExternalData(Context, StateTree, Descs, OutDataViews);
}

void UThistleStateTreeLease::OnRegister()
{
	Super::OnRegister();
	AttemptRegister();
}

bool UThistleStateTreeLease::RegistrationImplementation()
{
	auto bind = GetWorld()->GetSubsystem<UThistleBehavioralist>();
	if (bind)
	{
		//we are picked up during REGISTER ENEMY.
		return true;
	}
	else
	{
		return false;
	}
}

void UThistleStateTreeLease::OnClusterMarkedAsPendingKill()
{
	IsReady = false;
	bIsRunning = false;
	Super::OnClusterMarkedAsPendingKill();
}

void UThistleStateTreeLease::OnComponentDestroyed(bool bDestroyingHierarchy)
{
	IsReady = false;
	bIsRunning = false;
	Super::OnComponentDestroyed(bDestroyingHierarchy);
}

UThistleStateTreeLease::UThistleStateTreeLease(const FObjectInitializer& ObjectInitializer): Super(ObjectInitializer),
                                                                                             CurrentRunStatus()
{
	bWantsInitializeComponent = true;
	bIsRunning = true;
	bIsPaused = false;
	bStartLogicAutomatically = true;
}

UThistleStateTreeLease::~UThistleStateTreeLease()
{
	IsReady = false;
}

void UThistleStateTreeLease::BeginPlay()
{
	Super::BeginPlay();

	auto owner = GetOwner();
	BindDel = FOnCollectStateTreeExternalData::CreateUObject(this, &UThistleStateTreeLease::CollectExternalData);
	InstanceData = FStateTreeInstanceData();
	FStateTreeExecutionContext Context(*owner, *StateTreeRef.GetStateTree(), InstanceData);

	Context.Start();
	AttemptRegister();
}



FSkeletonKey UThistleStateTreeLease::GetMyKey() const
{
	IKeyedConstruct* KeyedConstruct = Cast<IKeyedConstruct>(GetOwner());
	return KeyedConstruct ? KeyedConstruct->GetMyKey() : FSkeletonKey();
}

UGameplayTasksComponent* UThistleStateTreeLease::GetGameplayTasksComponent(const UGameplayTask& Task) const
{
	UE_LOG(LogTemp, Error,
	       TEXT(
		       "UStateShrub runs in Artillery cadence and just tried to provide a gameplay tasks component from main thread. This could be real bad."
	       ));
	return Super::GetGameplayTasksComponent(Task);
}

bool UThistleStateTreeLease::SetContextRequirements(FStateTreeExecutionContext& Context, bool bLogErrors)
{
	Context.SetLinkedStateTreeOverrides(&LinkedStateTreeOverrides);
	InstanceOwnerKey = GetMyKey(); // failsafe.
	Context.SetCollectExternalDataCallback(BindDel);
	return UThistleStateTreeSchema::SetContextRequirements(GetMyKey(), Context) &&
		UStateTreeComponentSchema::SetContextRequirements(*this, Context);
}

void UThistleStateTreeLease::TickComponent(float DeltaTime, enum ELevelTick TickType,
                                           FActorComponentTickFunction* ThisTickFunction)
{
	if (!IsReady)
	{
		AttemptRegister();
	}
	else
	{
		SetComponentTickEnabled(false);
	}
}

void UThistleStateTreeLease::ArtilleryTick(uint64_t TicksSoFar)
{
	if (this && GetOwner() && IsReady)
	{
		if (MessagesToProcess.Num() > 0)
		{
			const int32 NumMessages = MessagesToProcess.Num();
			for (int32 Idx = 0; Idx < NumMessages; Idx++)
			{
				// create a copy of message in case MessagesToProcess is changed during loop
				const FAIMessage MessageCopy(MessagesToProcess[Idx]);

				for (int32 ObserverIndex = 0; ObserverIndex < MessageObservers.Num(); ObserverIndex++)
				{
					MessageObservers[ObserverIndex]->OnMessage(MessageCopy);
				}
			}
			MessagesToProcess.RemoveAt(0, NumMessages, EAllowShrinking::No);
		}
		InstanceOwnerKey = GetMyKey(); //freshen up, me hearties, yo ho.
		FStateTreeReference IncrementRefAsGuard = StateTreeRef; //Don't delete this. No, seriously. Don't you dare.
		FStateTreeExecutionContext Context(*GetOwner(), *IncrementRefAsGuard.GetStateTree(), InstanceData);

		if (!bIsRunning || bIsPaused)
		{
			return;
		}

		if (!StateTreeRef.IsValid())
		{
			return;
		}
		if (SetContextRequirements(Context))
		{
			const EStateTreeRunStatus PreviousRunStatus = Context.GetStateTreeRunStatus();
			if (PreviousRunStatus != EStateTreeRunStatus::Unset && Context.IsValid() //fine. FINE.
				&& Context.GetMutableInstanceData()->GetExecutionState()->TreeRunStatus != EStateTreeRunStatus::Unset
				 && bIsRunning && GetOwner() && GetWorld() && IsReady)
			{
				CurrentRunStatus = Context.Tick(1.0 / ArtilleryTickHertz);
			}
		}
	}
}

////////////////////////
