// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "CoreTypes.h"
#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "AttributeSet.h"
#include <unordered_map>

#include "GameplayAbilitySpec.h"
#include "GameplayAbilitySpecHandle.h"
#include "FArtilleryGun.h"
#include "CanonicalInputStreamECS.h"
#include "ArtilleryDispatch.h"
#include <bitset>
#include "ArtilleryCommonTypes.h"
#include "FAttributeMap.h"
#include "UArtilleryGameplayTagContainer.h"
#include "TransformDispatch.h"
#include "Components/ActorComponent.h"
#include "ArtilleryControlComponent.generated.h"

//Base class for player, enemy, and system state machines for controlling guns, managing binds, and integrating with actors.
//Also supports GAS legacy code.
struct FArtilleryGun;
UCLASS()
class ARTILLERYRUNTIME_API UArtilleryFireControl : public UAbilitySystemComponent
{
	friend FArtilleryGun;
	GENERATED_BODY()
	public:
	static inline int orderInInitialize = 0;
	UPROPERTY(BlueprintReadOnly)
	UArtilleryGameplayTagContainer* MyTags;
	UArtilleryDispatch* MyDispatch;
	UTransformDispatch* TransformDispatch;
	ActorKey ParentKey;
	//this needs to be replicated in iris, interestin'ly.
	TSet<FGunKey> MyGuns;

	TSharedPtr<FAttributeMap> MyAttributes;
	FireControlKey MyKey;
	bool Usable = false;

	virtual void PushGunToFireMapping(const FGunKey& ToFire) 
	{
		Arty::FArtilleryFireGunFromDispatch Inbound;
		Inbound.BindUObject(this, &UArtilleryFireControl::FireGun);
		MyDispatch->RegisterReady(ToFire, Inbound);
		MyGuns.Add(ToFire);
	}
	//it is strongly recommended that you understand
	// FGameplayAbilitySpec and FGameplayAbilitySpecDef, as well as Handle.
	// I'm deferring the solve for how we use them for now, in a desperate effort to
	// make sure we can preserve as much of the ability framework as possible
	// but spec management is going to be mission critical for determinism
	void FireGun(TSharedPtr<FArtilleryGun> Gun, bool InputAlreadyUsedOnce, ArtIPMKey FireAction)
	{
		if (Gun->Prefire != nullptr)
		{
			FGameplayAbilitySpec BackboneFiring = BuildAbilitySpecFromClass(
				(Gun->Prefire).GetClass(),
				0,
				-1
			);
			FGameplayAbilitySpecHandle FireHandle = BackboneFiring.Handle;
			Gun->PreFireGun(
				FireHandle,
				AbilityActorInfo.Get(),
				FGameplayAbilityActivationInfo(EGameplayAbilityActivationMode::Authority),
				FireAction);
		}
	};

	virtual void PopGunFromFireMapping(const FGunKey& ToRemove) 
	{
		MyDispatch->Deregister(ToRemove);
		MyGuns.Remove(ToRemove);
	}
	void InitializeComponent() override
	{
		Super::InitializeComponent();
		MyKey = UArtilleryFireControl::orderInInitialize++;
		//we rely on attribute replication, which I think is borderline necessary, but I wonder if we should use effect replication.
		//historically, relying on gameplay effect replication has led to situations where key state was not managed through effects.
		//for OUR situation, where we have few attributes and many effects, huge amounts of effects are likely not interesting for us to replicate.
		ReplicationMode = EGameplayEffectReplicationMode::Minimal; 
	};
	//this happens post init but pre begin play, and the world subsystems should exist by this point.
	//we use this to help ensure that if the actor's begin play triggers first, things will be set correctly
	//I've left the same code in begin play as a fallback.
	virtual void ReadyForReplication() override
	{
		Super::ReadyForReplication();
		MyDispatch = GetWorld()->GetSubsystem<UArtilleryDispatch>();
	}
	
	//We don't have as many phases on our components as we do actors. The bool Usable helps control our state instead.
	//This is, ironically, not a problem in actual usage, only testing, for us.
	virtual void BeginPlay() override
	{
		Super::BeginPlay(); 
		MyDispatch = GetWorld()->GetSubsystem<UArtilleryDispatch>();
	};
	
	virtual void OnComponentDestroyed(bool bDestroyingHierarchy) override
	{
		Super::OnComponentDestroyed(bDestroyingHierarchy);
		for (FGunKey Gun : MyGuns)
		{
			MyDispatch->Deregister(Gun); // emergency deregister.
		}
	};
	
};

//Notes from UFireControlMachine. These are from early in dev, so may be a lil dated. Still v useful. 
/** Who to route replication through if ReplicationProxyEnabled (if this returns null, when ReplicationProxyEnabled, we wont replicate)  */
//this is the tooling used in part by NPP's implementation, and we should consider using it as well to integrate with Iris and constrain
//replication to attributes only.
//virtual IAbilitySystemReplicationProxyInterface* GetReplicationInterface();


// These properties, and the fact that they are NOT the same, is important for understanding the model of the original
// ability system component. Note that this means by default, this component has no structural model for multi actor
// groups that might share abilities, such as enemy squads. While this sounds like a corner case, it is not.
// /** The actor that owns this component logically */
// UPROPERTY(ReplicatedUsing = OnRep_OwningActor)
// TObjectPtr<AActor> OwnerActor;
//
// /** The actor that is the physical representation used for abilities. Can be NULL */
// UPROPERTY(ReplicatedUsing = OnRep_OwningActor)
// TObjectPtr<AActor> AvatarActor;

/*/** Will be called from GiveAbility or from OnRep. Initializes events (triggers and inputs) with the given ability #1#
virtual void OnGiveAbility(FGameplayAbilitySpec& AbilitySpec);

/** Will be called from RemoveAbility or from OnRep. Unbinds inputs with the given ability #1#
virtual void OnRemoveAbility(FGameplayAbilitySpec& AbilitySpec);

/** Called from ClearAbility, ClearAllAbilities or OnRep. Clears any triggers that should no longer exist. #1#
void CheckForClearedAbilities();

/** Cancel a specific ability spec #1#
virtual void CancelAbilitySpec(FGameplayAbilitySpec& Spec, UGameplayAbility* Ignore);

/** Creates a new instance of an ability, storing it in the spec #1#
virtual UGameplayAbility* CreateNewInstanceOfAbility(FGameplayAbilitySpec& Spec, const UGameplayAbility* Ability);*/

