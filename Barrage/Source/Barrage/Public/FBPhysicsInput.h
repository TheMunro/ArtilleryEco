#pragma once
#include "IsolatedJoltIncludes.h"
#include "SkeletonTypes.h"
#include "FBPhysicsInputTypes.h"




//this struct must be size 48.
struct FBPhysicsInput
{
		FBarrageKey Target;
		uint64 Sequence;//unused, likely needed for determinism.
		PhysicsInputType Action;
		FBShape metadata;
		JPH::Quat State;
	
	explicit FBPhysicsInput(): Sequence(0), Action(), metadata(Uninitialized), State()
		{
			//don't initialize anything. just trust me on this.	
		}

		FBPhysicsInput(FBarrageKey Affected, int Seq, PhysicsInputType PhysicsInput, JPH::Quat Quat)
	{
		State = Quat;
		Target  = Affected;
		Sequence = Seq;
		Action = PhysicsInput;
		metadata = Uninitialized;
	};

};
