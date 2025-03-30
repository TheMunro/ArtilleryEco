#include "ThistleStateTreeAim.h"

#include "Kismet/KismetMathLibrary.h"

EStateTreeRunStatus FAimTurret::Tick(FStateTreeExecutionContext& Context, const float DeltaTime) const
{
	const FInstanceDataType& InstanceData = Context.GetInstanceData(*this);
	bool Shuck = false;
	auto location = InstanceData.ShuckPoi(Shuck);
	auto AreWeBarraging = UBarrageDispatch::SelfPtr;
	
	if (AreWeBarraging != nullptr)
	{
		bool found = true;
		auto HereIAm = UArtilleryLibrary::implK2_GetLocation(InstanceData.KeyOf, found);
		if (found)
		{
			if (!Shuck)
			{
				return EStateTreeRunStatus::Failed;
			}
			auto Rot = UKismetMathLibrary::FindLookAtRotation(HereIAm, location);
			auto MyRot = UArtilleryLibrary::implK2_GetAttr3Ptr(InstanceData.KeyOf, E_VectorAttrib::AimVector, found);
			if (found)
			{
				if (MyRot->CurrentValue.Equals(Rot.Vector(), 0.2f))
				{
					return EStateTreeRunStatus::Succeeded;
				}
			}
			else
			{
				// we do need to handle this but I actually don't know what we should do yet.
			}

				auto unit = MyRot->CurrentValue.GetSafeNormal();
				auto unit2 = Rot.Vector();
				auto dotto = unit.Dot(unit2);
				auto degrees = FMath::RadiansToDegrees(acos(dotto));
				if (degrees > 60)
				{
				  Rot =	FVector::SlerpVectorToDirection(unit, unit2, 0.4).Rotation();
				}
				UThistleBehavioralist::AttemptAimFromKey(InstanceData.KeyOf, Rot);
			return EStateTreeRunStatus::Running;
		}
	}
	return EStateTreeRunStatus::Failed;
}
