﻿#include "StaticAssetLoader.h"

#include "FArtilleryGun.h"
#include "FGunDefinitionRow.h"

void UStaticGunLoader::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	Definitions = Cast<UDataTable>(StaticLoadObject(UDataTable::StaticClass(), nullptr,
	                                                TEXT("DataTable'/Game/DataTables/GunDefinitions.GunDefinitions'")));

	if (Definitions == nullptr)
	{
		UE_LOG(LogDataTable, Error, TEXT("UStaticGunLoader::Initialize : No data table found at /Game/DataTables/GunDefinitions.GunDefinitions"));
		return;
	}

	Definitions->ForeachRow<FGunDefinitionRow>(
		TEXT("UStaticGunLoader::Initialize"),
		[this](const FName& Key, const FGunDefinitionRow& RowDefinition) mutable
	{
		if (RowDefinition.IsCPP)
		{
			const FString Bind = RowDefinition.LoadableCPP;
			FTopLevelAssetPath LoadFrom = FTopLevelAssetPath(Bind);
			// TODO - Add debugging tools to allow toggling messages like this
			// enough get sent that it is starting to affect performance
			//UE_LOG(LogTemp, Warning, TEXT("GunLoader: Loading from %s"), *Bind);
			if (LoadFrom.IsValid())
			{
				//UE_LOG(LogTemp, Warning, TEXT("GunLoader: Loading..."));
				UScriptStruct* StructMetadata = FindObject<UScriptStruct>(LoadFrom, false);
				//for reference, this log line + input
				//UScriptStruct* pointofcomp = FArtilleryGun::StaticStruct();
				//UE_LOG(LogTemp, Warning, TEXT("GunLoader: FArtilleryGun Info: [%s], [%s]"), *pointofcomp->GetStructCPPName(), *pointofcomp->GetStructPathName().ToString());
				//produces...
				//GunLoader: FArtilleryGun Info: [FArtilleryGun], [/Script/ArtilleryRuntime.ArtilleryGun]
				//And a similar line produces
				// [FGunWeeRocket], [/Script/Bristle54.GunWeeRocket]
				
				// TODO: make sure we don't need to flip this over to LoadObject. That'd be a real hassle.
				if (StructMetadata)
				{
					//UE_LOG(LogTemp, Warning, TEXT("GunLoader: Loaded UScriptStruct: [%s]"), *StructMetadata->GetName());
					void* container = FMemory::Malloc(StructMetadata->GetStructureSize());
					StructMetadata->InitializeStruct(container); //init & constructor

					//UE_LOG(LogTemp, Warning, TEXT("GunLoader: Initing USSI..."));
					TSharedPtr<FArtilleryGun> Form = MakeShareable(static_cast<FArtilleryGun*>(container));
					if (Form.IsValid())
					{
						ZardozMapping.Add(Bind, StructMetadata);
						
						FString InstanceOfPath = FString(RowDefinition.LoadableCPP, 0);
						FString InstanceOfId = FString(RowDefinition.GunDefinitionId, 0);
						//UE_LOG(LogTemp, Warning, TEXT("GunLoader: Loaded via [%s] with shortname: [%s]"),
						//*StructMetadata->GetAuthoredName(), *InstanceOfId);
						//slack can be quite large and this guarantees we store a copy.
						CommonNameToProperNameMapping.Add(InstanceOfId, InstanceOfPath);
					}
				}
			}
		}
		else
		//right now we don't handle hybrids. if you want C++ gun functionality in BP, the gun be a blueprint class that inherits from the C++ class
		//and to do this, you'll want to deeply understand the difference between BlueprintNativeEvent vs BlueprintImplementableEvent and sibling pairs.
		{
			//we don't handle bp stuff yet, sorry.
		}
	});
}

void UStaticGunLoader::Deinitialize()
{
	Super::Deinitialize();
}

TSharedPtr<FArtilleryGun> UStaticGunLoader::GetNewInstanceUninitialized(FString RequestedGunDefinitionID)
{
	FString* TrueName = CommonNameToProperNameMapping.Find(RequestedGunDefinitionID);
	if (TrueName)
	{
		UScriptStruct** GhostlyGun = ZardozMapping.Find(*TrueName);
		if (GhostlyGun)
		{
			UScriptStruct* BindMetadata = *GhostlyGun;
			if (GhostlyGun) //todo: figure out if this is a serious lifecycle risk. As these are available during game instance initialization, I think we're okay? but...
			{
				//UE_LOG(LogTemp, Warning, TEXT("GunLoader: Loaded UScriptStruct: [%s]"), *BindMetadata->GetName());
				void* container = FMemory::Malloc(BindMetadata->GetStructureSize());
				BindMetadata->InitializeStruct(container); //init & constructor

				//UE_LOG(LogTemp, Warning, TEXT("GunLoader: Initing USSI..."));
				return MakeShareable(static_cast<FArtilleryGun*>(container));
			}
		}
	}
	return nullptr;
}

UStaticGunLoader::~UStaticGunLoader()
{
}
