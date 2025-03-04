#include "ThistleStateTreeFire.h"

#include "Kismet/KismetMathLibrary.h"

EStateTreeRunStatus FFireTurret::Tick(FStateTreeExecutionContext& Context, const float DeltaTime) const
{
	const FInstanceDataType& InstanceData = Context.GetInstanceData(*this);
	auto AreWeBarraging = UBarrageDispatch::SelfPtr;
	
	if (AreWeBarraging != nullptr)
	{
		bool found = true;
		UArtilleryLibrary::implK2_GetLocation(InstanceData.KeyOf, found); // used as an existence check, sue me.
		if (found)
		{
			UThistleBehavioralist::AttemptAttackFromKey(InstanceData.KeyOf);
			return EStateTreeRunStatus::Succeeded;
		}

	}
	
	return EStateTreeRunStatus::Failed;
}
