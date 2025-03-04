#include "ORDIN.h"

#include "Math/UnitConversion.h"

UOrdinatePillar::UOrdinatePillar()
{
	SelfPtr = this;
	if (!ORDINATION_Fallback.burnt)
	{
		ORDINATION_Fallback.burnt = true;
		auto& A = Data;
		Data = ORDINATION_Fallback;
		ORDINATION_Fallback = A;
	}
}

UOrdinatePillar::~UOrdinatePillar()
{
	//we share our lifetime with all world subsystems.
	ORDINATION_Fallback.burnt = false;
	Data.Subsystems.Empty();
	//and the fallback
	ORDINATION_Fallback.Subsystems.Empty();
}

void UOrdinatePillar::Deinitialize()
{
	Super::Deinitialize();
	//we init and deinit on load reload...
	for (ORDIN::InitSequence* Group : Data.GROUPS)
	{
		Group->Empty();
	}
	//we also fry the fallback.
	for (ORDIN::InitSequence* Group : ORDINATION_Fallback.GROUPS)
	{
		Group->Empty();
	}
}


void UOrdinatePillar::REGISTERLORD(int RegisterAs, ISkeletonLord* YourThisPointer)
{
	if (this)
	{
		Data.Subsystems.Add(ORDIN::SubsystemKey(RegisterAs, YourThisPointer));
	}
	else if (!ORDINATION_Fallback.burnt)
	{
		ORDINATION_Fallback.Subsystems.Add(ORDIN::SubsystemKey(RegisterAs, YourThisPointer));
	}
}

void UOrdinatePillar::REGISTERORDER(int RegisterAs, int group, IKeyedConstruct* YourThisPointer)
{
	if (GIsRunning && ORDINATION_Fallback.burnt && this && GetWorld())
	{
		auto WContextType = GetWorld()->WorldType;
		if (WContextType == EWorldType::PIE || WContextType == EWorldType::Game)
		{
			if (group > 10 || group < 0)
			{
				throw std::invalid_argument("Invalid group");
			}
			auto forcealloc = ORDIN::SequencedKey(RegisterAs, YourThisPointer);
			Data.GROUPS[group]->Add(forcealloc);
		}
	}
	else if (!ORDINATION_Fallback.burnt)
	{
		if (group > 10 || group < 0)
		{
			throw std::invalid_argument("Invalid group");
		}
		ORDINATION_Fallback.GROUPS[group]->Add(ORDIN::SequencedKey(RegisterAs, YourThisPointer));
	}
}

void UOrdinatePillar::PostInitialize()
{
	if (GetWorld() && GetWorld()->IsGameWorld())
	{
		Super::PostInitialize();
		Data.Subsystems.Sort();
		for (ORDIN::SubsystemKey Register : Data.Subsystems)
		{
			Register.Value->IsReady = false;
			Register.Value->AttemptRegister();
		}
	}
}

void UOrdinatePillar::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
}

void UOrdinatePillar::OnWorldBeginPlay(UWorld& InWorld)
{
	Super::OnWorldBeginPlay(InWorld);
	for (ORDIN::InitSequence* Group : Data.GROUPS)
	{
		Group->Sort();
		for (ORDIN::SequencedKey Register : *Group)
		{
			Register.Value->AttemptRegister();
		}
	}
}
