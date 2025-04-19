// Fill out your copyright notice in the Description page of Project Settings.
#include "ThistleEvaluators.h"
#include "StateTreeExecutionContext.h"

bool FThistleGetPlayerKey::Link(FStateTreeLinker& Linker)
{
	return true; // artillery is... always ready, weirdly.
}

void FThistleGetPlayerKey::Tick(FStateTreeExecutionContext& Context, const float DeltaTime) const
{
	FInstanceDataType& InstanceData = Context.GetInstanceData(*this);

	//TODO: be great to have a version that doesn't go boom.
	InstanceData.OutputKey = UArtilleryLibrary::GetLocalPlayerKey_LOW_SAFETY();
}

void FThistleKeyToRelationship::Tick(FStateTreeExecutionContext& Context, const float DeltaTime) const
{
	FInstanceDataType& InstanceData = Context.GetInstanceData(*this);
	bool found = false;
	InstanceData.OutputKey = UArtilleryLibrary::implK2_GetIdentity(InstanceData.InputKey, InstanceData.Relationship,
	                                                               found);
	InstanceData.Found = found;
}

void FThistleSphereCast::Tick(FStateTreeExecutionContext& Context, const float DeltaTime) const
{
	FInstanceDataType& InstanceData = Context.GetInstanceData(*this);
	bool Shucked = false;
	auto Source = InstanceData.Source.ShuckPoi(Shucked);
	if (!Shucked) { return; }
	auto Target = InstanceData.Target.ShuckPoi(Shucked);
	// ReSharper disable once CppRedundantControlFlowJump
	if (!Shucked) { return; }
	auto ToFrom = (Target - Source);
	auto Length = ToFrom.Length() + 1;
	if ((UArtilleryLibrary::GetTotalsTickCount() % InstanceData.TicksBetweenCastRefresh) == 0 &&
		UBarrageDispatch::SelfPtr)
	{
		const JPH::DefaultBroadPhaseLayerFilter default_broadphase_layer_filter = UBarrageDispatch::SelfPtr->JoltGameSim
			->physics_system->GetDefaultBroadPhaseLayerFilter(Layers::CAST_QUERY);
		const JPH::DefaultObjectLayerFilter default_object_layer_filter = UBarrageDispatch::SelfPtr->JoltGameSim->
			physics_system->GetDefaultLayerFilter(Layers::CAST_QUERY);

		if (InstanceData.SourceBodyKey_SetOrRegret.IsValid()
			&& UArtilleryDispatch::SelfPtr->IsLiveKey(InstanceData.SourceBodyKey_SetOrRegret) != DEAD)
		{
			JPH::IgnoreSingleBodyFilter StopHittingYourself = UBarrageDispatch::SelfPtr->GetFilterToIgnoreSingleBody(
				UBarrageDispatch::SelfPtr->GetShapeRef(InstanceData.SourceBodyKey_SetOrRegret)->KeyIntoBarrage);
			UBarrageDispatch::SelfPtr->SphereCast(InstanceData.Radius, Length, Source, ToFrom.GetSafeNormal(),
			                                      InstanceData.HitResultCache, default_broadphase_layer_filter,
			                                      default_object_layer_filter, StopHittingYourself);
			InstanceData.Outcome = *(InstanceData.HitResultCache);
		}
		else
		{
			UBarrageDispatch::SelfPtr->SphereCast(InstanceData.Radius, Length, Source, ToFrom.GetSafeNormal(),
			                                      InstanceData.HitResultCache, default_broadphase_layer_filter,
			                                      default_object_layer_filter, JPH::BodyFilter());
			InstanceData.Outcome = *(InstanceData.HitResultCache);
		}
	}
	else
	{
		InstanceData.Outcome = *(InstanceData.HitResultCache);
	}
}
