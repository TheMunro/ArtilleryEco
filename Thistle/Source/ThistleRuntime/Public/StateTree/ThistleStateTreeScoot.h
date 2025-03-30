#pragma once
#include "MassStateTreeSchema.h"
#include "StateTreeTaskBase.h"		//this defines the basic form of an actual tree task.
#include "StateTreeExecutionContext.h"
#include "ThistleBehavioralist.h"
#include "ThistleDispatch.h"
#include "ThistleStateTreeSchema.h"
#include "ThistleTypes.h"
#include "ThistleStateTreeCore.h"
#include "Components/StateTreeComponent.h"
#include "ThistleStateTreeScoot.generated.h"

using namespace ThistleTypes;

//Scoots away whenever distance to PoI is less than tolerance on a slow cadence.
USTRUCT()
struct THISTLERUNTIME_API FScoot : public FTTaskBase
{

	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere)
	float Tolerance = 200;

	using FInstanceDataType = F_TPOIInstanceNavData;

protected:
	virtual EStateTreeRunStatus Tick(FStateTreeExecutionContext& Context, const float DeltaTime) const override;
	EStateTreeRunStatus AttemptScootPath(FStateTreeExecutionContext& Context, FVector location, FVector HereIAm) const;
	virtual const UStruct* GetInstanceDataType() const override { return FInstanceDataType::StaticStruct(); }
};

USTRUCT()
struct THISTLERUNTIME_API FBreakOff : public FTTaskBase
{

	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere)
	float Tolerance = 200;

	using FInstanceDataType = F_TPOIInstanceNavData;

protected:
	virtual EStateTreeRunStatus Tick(FStateTreeExecutionContext& Context, const float DeltaTime) const override;
	virtual const UStruct* GetInstanceDataType() const override { return FInstanceDataType::StaticStruct(); }
};