// Copyright 2024 Oversized Sun Inc. All Rights Reserved.

#include "ArtilleryProjectileDispatch.h"
#include "BarrageContactEvent.h"
#include "ArtilleryBPLibs.h"
#include "BarrageDispatch.h"
#include "FArtilleryGun.h"

void UArtilleryProjectileDispatch::ArtilleryTick()
{
	//On Tick, we see if anybody needs to go.
	++ExpirationCounter;
	if (ExpirationDeadliner->Contains(ExpirationCounter))
	{
		TArray<FSkeletonKey> AnyToExpire = ExpirationDeadliner->FindAndRemoveChecked(ExpirationCounter);
		for (FSkeletonKey Goner : AnyToExpire)
		{
			DeleteProjectile(Goner);
		}
	}
}

bool UArtilleryProjectileDispatch::RegistrationImplementation()
{
	// TODO: Can we find and autoload the datatable, or do
	ProjectileDefinitions = LoadObject<UDataTable>(nullptr, TEXT("DataTable'/Game/DataTables/ProjectileDefinitions.ProjectileDefinitions'"));

	UE_LOG(LogTemp, Warning, TEXT("ArtilleryProjectileDispatch:Subsystem: Online"));
	ProjectileDefinitions->ForeachRow<FProjectileDefinitionRow>(
		TEXT("UArtilleryProjectileDispatch::PostInitialize"),
		[this](const FName& Key, const FProjectileDefinitionRow& ProjectileDefinition) mutable
	 {
		 UNiagaraParticleDispatch* NPD = GetWorld()->GetSubsystem<UNiagaraParticleDispatch>();
		 check(NPD);
		 if (UStaticMesh* StaticMeshPtr = LoadObject<UStaticMesh>(nullptr, *ProjectileDefinition.ProjectileMeshLocation))
		 {
			 if (!MeshAssetToMeshManagerMapping->Contains(*ProjectileDefinition.ProjectileMeshLocation))
			 {
				 AInstancedMeshManager* NewMeshManager = GetWorld()->SpawnActor<AInstancedMeshManager>();
			 	if (NewMeshManager == nullptr)
			 	{
			 		UE_LOG(LogTemp, Error, TEXT("Could not spawn mesh manager actor. If this is during editor load, it can be ignored. Otherwise..."));
			 	}
				 NewMeshManager->InitializeManager();
				 NewMeshManager->SetStaticMesh(StaticMeshPtr);
				 NewMeshManager->SetInternalFlags(EInternalObjectFlags::Async);
				 NewMeshManager->SwarmKineManager->SetCanEverAffectNavigation(false);
				 NewMeshManager->SwarmKineManager->SetSimulatePhysics(false);
				 NewMeshManager->SwarmKineManager->bNavigationRelevant = 0;
			 	 NewMeshManager->SwarmKineManager->SetCollisionEnabled(ECollisionEnabled::Type::NoCollision);
				 NewMeshManager->SwarmKineManager->SetInternalFlags(EInternalObjectFlags::Async);

				 ManagerKeyToMeshManagerMapping->Add(NewMeshManager->GetMyKey(), NewMeshManager);
				 ProjectileNameToMeshManagerMapping->Add(FName(ProjectileDefinition.ProjectileDefinitionId),
				                                         NewMeshManager);
				 MeshAssetToMeshManagerMapping->Add(*ProjectileDefinition.ProjectileMeshLocation, NewMeshManager);
				 if (!ProjectileDefinition.ParticleEffectDataChannel.IsEmpty())
				 {
					 NPD->AddNamedNDCReference(*ProjectileDefinition.ProjectileDefinitionId,
					                           *ProjectileDefinition.ParticleEffectDataChannel);
				 }
			 	
			 	//these are standing in for a pretty messy body of more specific work we could do instead.
			 	//basically, when you load up a new mesh, there's quite a bit of work and book-keeping that you
			 	//need to do for instanced static mesh systems because loading a mesh from disk does one thing in editor
			 	//and something else in non-PIE sessions. There are VERY good reasons for this, but it means you need
			 	//a lot of specialization. Or you can just NUKE THE SITE FROM ORBIT. which seems to work. and is brainless.
			 	//and just as performant. welp.
			 	NewMeshManager->SwarmKineManager->OnMeshRebuild(true);
			 	NewMeshManager->SwarmKineManager->ReregisterComponent();
			 }
			 else
			 {
			 	TWeakObjectPtr<AInstancedMeshManager>* MeshManager = MeshAssetToMeshManagerMapping->Find(*ProjectileDefinition.ProjectileMeshLocation);
			 	if (MeshManager)
			 	{
			 		ProjectileNameToMeshManagerMapping->Add(FName(ProjectileDefinition.ProjectileDefinitionId),
					                                         *MeshManager);
			 	}
				else
				{
					throw; // we just checked this, you monster. 
				}
			 }
		 }
	 });

	MyDispatch = GetWorld()->GetSubsystem<UArtilleryDispatch>();
	check(MyDispatch);
	UBarrageDispatch* BarrageDispatch = GetWorld()->GetSubsystem<UBarrageDispatch>();
	BarrageDispatch->OnBarrageContactAddedDelegate.AddUObject(this, &UArtilleryProjectileDispatch::OnBarrageContactAdded);
	UArtilleryDispatch::SelfPtr->SetProjectileDispatch(this);
	SelfPtr = this;
	return true;
}

void UArtilleryProjectileDispatch::Initialize(FSubsystemCollectionBase& Collection)
{
	SET_INITIALIZATION_ORDER_BY_ORDINATEKEY_AND_WORLD
	Super::Initialize(Collection);
}

void UArtilleryProjectileDispatch::PostInitialize()
{
	Super::PostInitialize();
}

void UArtilleryProjectileDispatch::OnWorldBeginPlay(UWorld& InWorld)
{
	Super::OnWorldBeginPlay(InWorld);
}

void UArtilleryProjectileDispatch::Deinitialize()
{
	TSharedPtr<KeyToItemCuckooMap> HoldOpen = ProjectileKeyToMeshManagerMapping;
	TSharedPtr<TMap<FString, TWeakObjectPtr<AInstancedMeshManager>>> HoldOpenManagers = MeshAssetToMeshManagerMapping;
	ManagerKeyToMeshManagerMapping->Empty();
	ProjectileNameToMeshManagerMapping->Empty();
	ProjectileToGunMapping->clear();
	ExpirationDeadliner->Empty();
	ExpirationCounter = 0;
	if (HoldOpen)
	{
		KeyToItemCuckooMap::locked_table HoldOpenLocked = HoldOpen->lock_table();
		for (std::pair<FSkeletonKey, TWeakObjectPtr<AInstancedMeshManager>> RemainingKey : HoldOpenLocked)
		{
			TWeakObjectPtr<AInstancedMeshManager> Hold = RemainingKey.second;
			if (Hold != nullptr)
			{
				Hold->CleanupInstance(RemainingKey.first);
			}
		}
	}
	if (HoldOpenManagers)
	{
		// ReSharper disable once CppTemplateArgumentsCanBeDeduced - removing "redundant" template typing causes internal compiler error
		for (TTuple<FString, TWeakObjectPtr<AInstancedMeshManager>> RemainingKey : *HoldOpenManagers.Get())
		{
			AInstancedMeshManager* Hold = RemainingKey.Value.Get();
			if (Hold)
			{
				Hold->SwarmKineManager->ClearInternalFlags(EInternalObjectFlags::Async);
				Hold->ClearInternalFlags(EInternalObjectFlags::Async);
				Hold->ConditionalBeginDestroy();
			}
		}
	}

	ProjectileKeyToMeshManagerMapping->clear();
	MeshAssetToMeshManagerMapping->Empty();
	Super::Deinitialize();
}

UArtilleryProjectileDispatch::UArtilleryProjectileDispatch(): ProjectileDefinitions(nullptr), MyDispatch(nullptr)
{
	ExpirationCounter = 0; //just to make it clear.
	ManagerKeyToMeshManagerMapping = MakeShareable(new TMap<FSkeletonKey, TWeakObjectPtr<AInstancedMeshManager>>());
	ProjectileKeyToMeshManagerMapping = MakeShareable(new KeyToItemCuckooMap());
	ExpirationDeadliner = MakeShareable(new TSortedMap<int, TArray<FSkeletonKey>>());
	ProjectileNameToMeshManagerMapping = MakeShareable(new TMap<FName, TWeakObjectPtr<AInstancedMeshManager>>());
	MeshAssetToMeshManagerMapping = MakeShareable(new TMap<FString, TWeakObjectPtr<AInstancedMeshManager>>());
	ProjectileToGunMapping = MakeShareable(new libcuckoo::cuckoohash_map<FSkeletonKey, FGunKey>());
}

UArtilleryProjectileDispatch::~UArtilleryProjectileDispatch()
{
}

FProjectileDefinitionRow* UArtilleryProjectileDispatch::GetProjectileDefinitionRow(const FName ProjectileDefinitionId)
{
	if (ProjectileDefinitions != nullptr)
	{
		FProjectileDefinitionRow* FoundRow = ProjectileDefinitions->FindRow<FProjectileDefinitionRow>(
			ProjectileDefinitionId, TEXT("ProjectileTableLibrary"));
		return FoundRow;
	}
	return nullptr;
}

FSkeletonKey UArtilleryProjectileDispatch::QueueProjectileInstance(const FName ProjectileDefinitionId,
                                                                   const FGunKey& Gun, const FVector3d& StartLocation,
                                                                   const FVector3d& MuzzleVelocity, const float Scale,
                                                                   Layers::EJoltPhysicsLayer Layer,
                                                                   TArray<FGameplayTag>* TagArray) const
{
	TWeakObjectPtr<AInstancedMeshManager>* MeshManagerPtr = ProjectileNameToMeshManagerMapping->Find(ProjectileDefinitionId);
	if (MeshManagerPtr != nullptr && MeshManagerPtr->IsValid())
	{
		TWeakObjectPtr<AInstancedMeshManager> MeshManager = *MeshManagerPtr;
		FSkeletonKey ProjectileKey = MeshManager->GenerateNewProjectileKey();
		MyDispatch->RequestRouter->Bullet(ProjectileDefinitionId, StartLocation, Scale, MuzzleVelocity, ProjectileKey,
		                                  Gun, MyDispatch->GetShadowNow(), Layer);
		if (TagArray != nullptr)
		{
			GameplayTagContainerPtr TagContainer = MyDispatch->GetGameplayTagContainerAndAddIfNotExists(ProjectileKey);
			if (TagContainer.IsValid())
			{
				for (FGameplayTag& Tag : *TagArray)
				{
					TagContainer->AddTag(Tag);
				}
			}
		}
		return ProjectileKey;
	}
	return FSkeletonKey();
}

FSkeletonKey UArtilleryProjectileDispatch::CreateProjectileInstance(const FSkeletonKey& ProjectileKey,
                                                                    const FGunKey& Gun,
                                                                    const FName ProjectileDefinitionId,
                                                                    const FTransform& WorldTransform,
                                                                    const FVector3d& MuzzleVelocity,
                                                                    const float Scale,
                                                                    const bool IsSensor,
                                                                    const bool IsDynamic,
                                                                    Layers::EJoltPhysicsLayer Layer,
                                                                    const bool CanExpire,
                                                                    const int LifeInTicks) const
{
	TWeakObjectPtr<AInstancedMeshManager>* MeshManagerPtr = ProjectileNameToMeshManagerMapping->Find(ProjectileDefinitionId);
	if (MeshManagerPtr && MeshManagerPtr->IsValid())
	{
		TWeakObjectPtr<AInstancedMeshManager> MeshManager = *MeshManagerPtr;
		if (MeshManager.IsValid())
		{
			FSkeletonKey NewProjectileKey = MeshManager->CreateNewInstance(
				WorldTransform, MuzzleVelocity, Layer, Scale, ProjectileKey, IsSensor, IsDynamic);
			ProjectileKeyToMeshManagerMapping->insert(NewProjectileKey, MeshManager);
			ProjectileToGunMapping->insert(NewProjectileKey, Gun);

			UNiagaraParticleDispatch* NPD = GetWorld()->GetSubsystem<UNiagaraParticleDispatch>();
			check(NPD);
			TWeakObjectPtr<UNiagaraDataChannelAsset> ProjectileNDCAssetPtr = NPD->GetNDCAssetForProjectileDefinition(
				ProjectileDefinitionId.ToString());
			if (ProjectileNDCAssetPtr.IsValid())
			{
				ParticleRecord& NewParticleRecord = NPD->RegisterKeyForProcessing(NewProjectileKey);
				NewParticleRecord.NDCAssetPtr = ProjectileNDCAssetPtr;
				NewParticleRecord.NDCIndex = -1;
			}
			if (CanExpire)
			{
				//TODO: revisit to provide rollback support. it'll be exactly like tombstones.
				int ExpireTicks = LifeInTicks == -1 ? DEFAULT_LIFE_OF_PROJECTILE : LifeInTicks;
				TArray<FSkeletonKey>* ArrayIfAny = ExpirationDeadliner->Find(ExpirationCounter + ExpireTicks);
				if (ArrayIfAny != nullptr)
				{
					ArrayIfAny->Add(NewProjectileKey);
				}
				else
				{
					ExpirationDeadliner->Add(ExpirationCounter+ExpireTicks, {NewProjectileKey});
				}
			}
			return NewProjectileKey;
		}
	}
	UE_LOG(LogTemp, Error, TEXT("Could not find preloaded projectile instance manager with id %s"),
	       *ProjectileDefinitionId.ToString());
	return FSkeletonKey();
}

bool UArtilleryProjectileDispatch::IsArtilleryProjectile(const FSkeletonKey MaybeProjectile)
{
	return ProjectileKeyToMeshManagerMapping->contains(MaybeProjectile);
}

void UArtilleryProjectileDispatch::DeleteProjectile(const FSkeletonKey Target)
{
	TWeakObjectPtr<AInstancedMeshManager> MeshManager;
	ProjectileKeyToMeshManagerMapping->find(Target, MeshManager);
	if (MeshManager.IsValid())
	{
		MeshManager->CleanupInstance(Target);
		ProjectileKeyToMeshManagerMapping->erase(Target);
	}
}

TWeakObjectPtr<AInstancedMeshManager> UArtilleryProjectileDispatch::GetProjectileMeshManagerByManagerKey(
	const FSkeletonKey ManagerKey)
{
	TWeakObjectPtr<AInstancedMeshManager>* ManagerRef = ManagerKeyToMeshManagerMapping->Find(ManagerKey);
	return ManagerRef != nullptr ? *ManagerRef : nullptr;
}

TWeakObjectPtr<AInstancedMeshManager> UArtilleryProjectileDispatch::GetProjectileMeshManagerByProjectileKey(
	const FSkeletonKey ProjectileKey)
{
	TWeakObjectPtr<AInstancedMeshManager> ManagerRef;
	ProjectileKeyToMeshManagerMapping->find(ProjectileKey, ManagerRef);
	return ManagerRef;
}

TWeakObjectPtr<USceneComponent> UArtilleryProjectileDispatch::GetSceneComponentForProjectile(
	const FSkeletonKey ProjectileKey)
{
	TWeakObjectPtr<AInstancedMeshManager> ManagerRef;
	ProjectileKeyToMeshManagerMapping->find(ProjectileKey, ManagerRef);
	return ManagerRef.IsValid() ? ManagerRef->GetSceneComponentForInstance(ProjectileKey) : nullptr;
}

void UArtilleryProjectileDispatch::OnBarrageContactAdded(const BarrageContactEvent& ContactEvent)
{
	// We only care if one of the entities is a projectile
	if (ContactEvent.IsEitherEntityAProjectile())
	{
		bool Body1_IsBullet = ContactEvent.ContactEntity1.bIsProjectile;
		// if both are bullets, if their layers allow it, we will collide them.
		//this is actually how antimissile works.
		FSkeletonKey ProjectileKey = Body1_IsBullet ? ContactEvent.ContactEntity1.ContactKey : ContactEvent.ContactEntity2.ContactKey;
		FSkeletonKey EntityHitKey = Body1_IsBullet ? ContactEvent.ContactEntity2.ContactKey : ContactEvent.ContactEntity1.ContactKey;

		// Call in to this projectile's gun to handle collision logic
		FGunKey GunKey;
		ProjectileToGunMapping->find(ProjectileKey, GunKey);
		if (GunKey.IsValidInstance())
		{
			TSharedPtr<FArtilleryGun> ProjectileGun = MyDispatch->GetPointerToGun(GunKey);
			if (ProjectileGun.IsValid())
			{
				ProjectileGun.Get()->ProjectileCollided(ProjectileKey);
			}
		}

		// TODO: make action based on projectile configuration, not just 100 health damage
		UArtilleryLibrary::ApplyDamage(EntityHitKey, 100);
		UArtilleryLibrary::DeleteProjectile(ProjectileKey);
	}
}
