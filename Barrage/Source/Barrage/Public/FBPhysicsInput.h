// Copyright 2025 Oversized Sun Inc. All Rights Reserved.

#pragma once

#include "IsolatedJoltIncludes.h"
#include "FBPhysicsInputTypes.h"

// *********************** IMPORTANT ***********************
// This struct must be pragma pack immune in order to properly work in Jolt.
// Please be careful if the size needs to be changed
static constexpr int FB_PHYSICS_INPUT_EXPECTED_SIZE = 48;
// *********************************************************
struct FBPhysicsInput
{
	FBarrageKey Target;
	uint64 Sequence;//unused, likely needed for determinism.
	PhysicsInputType Action;
	FBShape metadata;
	JPH::Quat State;
	
	explicit FBPhysicsInput() = delete;

	FBPhysicsInput(FBarrageKey Affected, int Seq, PhysicsInputType PhysicsInput, JPH::Quat Quat, FBShape PrimitiveShape)
	{
		State = Quat;
		Target  = Affected;
		Sequence = Seq;
		Action = PhysicsInput;
		metadata = PrimitiveShape;
	};
};
static_assert(sizeof(FBPhysicsInput) == FB_PHYSICS_INPUT_EXPECTED_SIZE);
