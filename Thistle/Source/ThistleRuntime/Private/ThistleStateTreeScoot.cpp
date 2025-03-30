#include "ThistleStateTreeScoot.h"

#include "Public/GameplayTags.h"

EStateTreeRunStatus FScoot::Tick(FStateTreeExecutionContext& Context, const float DeltaTime) const
{
	const FInstanceDataType& InstanceData = Context.GetInstanceData(*this);
	bool Shuck = false;
	auto location = InstanceData.ShuckPoi(Shuck);
	if (!Shuck)
	{
		return EStateTreeRunStatus::Failed;
	}
		//run on cadence.
	if (UArtilleryLibrary::GetTotalsTickCount() % ArtilleryTickHertz*5 == 0)
	{
		bool found = false;
		auto HereIAm = UArtilleryLibrary::implK2_GetLocation(InstanceData.KeyOf, found);

		if (found && (HereIAm - location).Length() < Tolerance)
		{

			return AttemptScootPath(Context, location, HereIAm);
		}
		else
		{
			return EStateTreeRunStatus::Succeeded; //scoot not needed, is like succeeded.
		}
	}
	return EStateTreeRunStatus::Running;
}


EStateTreeRunStatus FScoot::AttemptScootPath(FStateTreeExecutionContext& Context, FVector location,
														   FVector HereIAm) const
{
	const FInstanceDataType& InstanceData = Context.GetInstanceData(*this);
	auto AreWeBarraging = UBarrageDispatch::SelfPtr;

	if (AreWeBarraging != nullptr && UThistleBehavioralist::SelfPtr)
	{
		bool found = false;
		auto tagc = UArtilleryLibrary::InternalTagsByKey(InstanceData.KeyOf, found);
		if (found)
		{
			tagc->RemoveTag(TAG_Orders_Move_Needed);
		}

		UThistleBehavioralist::SelfPtr->BounceTag(InstanceData.KeyOf, TAG_Orders_Move_Needed,
												  UThistleBehavioralist::DelayBetweenMoveOrders); 

		if ((HereIAm - location).Length() > FMath::Max(0.01f, Tolerance)*5)
		{
			if (found)
			{
				tagc->AddTag(TAG_Orders_Move_Needed);
			}
			return EStateTreeRunStatus::Succeeded;
		}
		
		auto destination = ( HereIAm - location).GetSafeNormal();
		if (destination != FVector::ZeroVector && !UThistleBehavioralist::AttemptInvokePathingOnKey(InstanceData.KeyOf,
		//move away to...
		(HereIAm + (Tolerance*10)*destination)))
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
			return EStateTreeRunStatus::Running;
		}
	}
	return EStateTreeRunStatus::Failed;
}

EStateTreeRunStatus FBreakOff::Tick(FStateTreeExecutionContext& Context, const float DeltaTime) const
{
	const FInstanceDataType& InstanceData = Context.GetInstanceData(*this);
	bool Shuck = false;
	auto location = InstanceData.ShuckPoi(Shuck);
	if (!Shuck)
	{
		return EStateTreeRunStatus::Failed;
	}
	//run on cadence.
	if (UArtilleryLibrary::GetTotalsTickCount() % 8 == 0)
	{
		bool found = false;
		auto HereIAm = UArtilleryLibrary::implK2_GetLocation(InstanceData.KeyOf, found);

		if (found && (HereIAm - location).Length()  < Tolerance * 2)
		{
		auto AreWeBarraging = UBarrageDispatch::SelfPtr;

		if (AreWeBarraging != nullptr && UThistleBehavioralist::SelfPtr)
		{
			auto tagc = UArtilleryLibrary::InternalTagsByKey(InstanceData.KeyOf, found);
			auto destination = ( HereIAm - location).GetSafeNormal();
			if (destination != FVector::ZeroVector && (HereIAm - location).Length()  < Tolerance * 1.1)
			{
				if (found)
				{
					UThistleBehavioralist::SelfPtr->BounceTag(InstanceData.KeyOf, TAG_Orders_Move_Needed,
										  UThistleBehavioralist::DelayBetweenMoveOrders);
					UThistleBehavioralist::SelfPtr->ExpireTag(InstanceData.KeyOf, TAG_Orders_Move_Break,
										  UThistleBehavioralist::DelayBetweenMoveOrders); 
				}           
				return EStateTreeRunStatus::Succeeded;
			}
			// ReSharper disable once CppRedundantElseKeywordInsideCompoundStatement
			else
			{
				return EStateTreeRunStatus::Running;
			}
		}
		}
		else
		{
			return EStateTreeRunStatus::Succeeded;
		}
	}
	return EStateTreeRunStatus::Running;
}
