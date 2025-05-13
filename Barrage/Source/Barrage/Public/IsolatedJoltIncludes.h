#pragma once
#include "HAL/Platform.h"
THIRD_PARTY_INCLUDES_START


PRAGMA_PUSH_PLATFORM_DEFAULT_PACKING
#include "mimalloc.h"
#include "Jolt/Jolt.h"
#include "Jolt/RegisterTypes.h"
#include "Jolt/Math/Quat.h"
#include "Jolt/Math/Vec3.h"
#include "PhysicsEngine/BodySetup.h"
#include "Jolt/Physics/PhysicsSystem.h"
#include "Jolt/Core/Factory.h"
#include "Jolt/Core/TempAllocator.h"
#include "Jolt/Core/JobSystemThreadPool.h"
#include "Jolt/Physics/PhysicsSettings.h"
#include "Jolt/Physics/Collision/Shape/BoxShape.h"
#include "Jolt/Physics/Collision/Shape/SphereShape.h"
#include "Jolt/Physics/Collision/Shape/MeshShape.h"
#include "Jolt/Physics/Body/BodyCreationSettings.h"
#include "Jolt/Physics/Body/BodyActivationListener.h"
#include "Jolt/Physics/Collision/Shape/CapsuleShape.h"
#include "Jolt/Physics/Collision/Shape/MeshShape.h"
#include <Jolt/Physics/Collision/RayCast.h>
#include <Jolt/Physics/Collision/ShapeCast.h>
#include "Jolt/Physics/Character/CharacterBase.h"
#include "Jolt/Physics/Character/CharacterVirtual.h"
#include "Jolt/Physics/Constraints/FixedConstraint.h"
#include "Jolt/Physics/Character/Character.h"
#include "Jolt/Physics/Collision/Shape/RotatedTranslatedShape.h"
#include "Jolt/Physics/Collision/CastResult.h"
#include "Jolt/ConfigurationString.h"
#include "libcuckoo/cuckoohash_map.hh"

struct FSkeletonKey;
struct FBarrageKey;

typedef libcuckoo::cuckoohash_map<FSkeletonKey, FBarrageKey> KeyToKey;

typedef libcuckoo::cuckoohash_map<uint64_t, JPH::Ref<JPH::Shape>> BoundsToShape;
typedef libcuckoo::cuckoohash_map<FBarrageKey, JPH::BodyID> KeyToBody;
// Disable common warnings triggered by Jolt, you can use JPH_SUPPRESS_WARNING_PUSH / JPH_SUPPRESS_WARNING_POP to store and restore the warning state
JPH_SUPPRESS_WARNINGS

PRAGMA_POP_PLATFORM_DEFAULT_PACKING
THIRD_PARTY_INCLUDES_END
#include <thread>
class IsolatedJoltIncludes
{
public:
	
};

namespace JOLT
{
	using namespace JPH;
	using namespace JPH::literals;

	namespace BroadPhaseLayers
	{
		static constexpr BroadPhaseLayer NON_MOVING(0);
		static constexpr BroadPhaseLayer MOVING(1);
		static constexpr BroadPhaseLayer DEBRIS(2);
		static constexpr uint NUM_LAYERS(3);
	};	
}
