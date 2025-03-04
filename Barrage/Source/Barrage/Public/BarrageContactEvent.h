#pragma once
#include "EPhysicsLayer.h"
#include "IsolatedJoltIncludes.h"

struct BarrageContactEntity
{
	BarrageContactEntity(const FSkeletonKey ContactKeyIn)
	{
		ContactKey = ContactKeyIn;
		bIsProjectile = false;
		bIsStaticGeometry = false;
		MyLayer = Layers::NUM_LAYERS;
	}
	BarrageContactEntity(const FSkeletonKey ContactKeyIn, const JPH::Body& BodyIn)
	{
		ContactKey = ContactKeyIn;
		bIsProjectile = BodyIn.GetObjectLayer() == Layers::PROJECTILE || BodyIn.GetObjectLayer() == Layers::ENEMYPROJECTILE;
		bIsStaticGeometry = BodyIn.GetObjectLayer() == Layers::NON_MOVING;
		MyLayer = Layers::EJoltPhysicsLayer(BodyIn.GetObjectLayer());
	}
	FSkeletonKey ContactKey;
	bool bIsProjectile:1;
	bool bIsStaticGeometry:1;
	Layers::EJoltPhysicsLayer MyLayer;
};

enum class EBarrageContactEventType
{
	ADDED,
	PERSISTED,
	REMOVED
};

struct BarrageContactEvent
{
	EBarrageContactEventType ContactEventType;
	BarrageContactEntity ContactEntity1;
	BarrageContactEntity ContactEntity2;

	bool IsEitherEntityAProjectile() const
	{
		return ContactEntity1.bIsProjectile || ContactEntity2.bIsProjectile;
	}
};