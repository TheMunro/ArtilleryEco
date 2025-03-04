#pragma once

#include "CoreMinimal.h"
#include "ArtilleryDispatch.h"
#include "EAttributes.h"
#include "EPhysicsLayer.h"
#include "GameFramework/Actor.h"
#include "FGunKey.h"
#include "RequestRouterTypes.h"
#include "NiagaraParticleDispatch.h"

//In general, you will not need to use this class directly.
//For some requests, or if you are building a Harvester Ticklite (see: https://github.com/JKurzer/sd/blob/main/README.md )
//It may be necessary. It is slower, bulkier, less immediate, and more annoying than the processes you are accustomed to,
//but for certain things that touch the game thread, it may be the best option. New stuff like that should get added here.
//it is also made available to autoguns as a way of shimming out some pretty nasty concurrency problems.

constexpr int ALLOWED_THREADS_FOR_ARTILLERY = 64;
//if we could make a promise about when threads are allocated, we could probably get rid of this
//since the accumulator is in the world subsystem and so gets cleared when the world spins down.
//This is identical to the design found in Barrage, since it ended up working beautifully.
thread_local static uint32 MyARTILLERYIndex = ALLOWED_THREADS_FOR_ARTILLERY + 1;


//these functions all create and enqueue a thing request and can be called from any thread that has called Feed().
//When F_INeedA is set up, these requests will go into the Threaded Accumulator (the BusyWorkerAcc machinery)...
//Then some thread will process the requests, likely the busy worker, doing the actual resolution.
//this is why the GrantedWith results have conditions that can be specified to let you know when or if the skeleton key is usable.
//Until the key is usable, it should not be separated from the GrantedWith structure or copied.
//I haven't spent a lot of time thinking about if the skeletonkey should get set later or if we should allocate a skeletonkey
//THEN try to build everything it represents, in this model.
//That would require an indirection mapping, which I find highly highly distasteful, but on the other hand, we're in
//extremely deep waters. On the grasping hand, everything we do to improve the UX NOW is a hundred bugs a year we don't have
//to _______________ ________ until ____ __ _________ and fix later.
// Find in files for YAGAMETHREADBOYRUNNETHREQUESTSHERE will show you the code in artillery dispatch that processes gamethread
// and YABUSYTHREADBOYRUNNETHREQUESTSHERE will show you the equivalent code in our mutual friend artillery busy worker.
class ARTILLERYRUNTIME_API F_INeedA //Frick
{
public:
	using GameThreadRequestQ = TCircularQueue<FRequestGameThreadThing>;
	using ThreadFeed = TCircularQueue<FRequestThing>;
	
	/// A 64 bit hash function by Thomas Wang, Jan 1997
	/// See: http://web.archive.org/web/20071223173210/http://www.concentric.net/~Ttwang/tech/inthash.htm
	/// @param inValue Value to hash
	/// @return Hash
	static inline uint64 FastHash64(uint64 inValue)
	{
		uint64 hash = inValue;
		hash = (~hash) + (hash << 21); // hash = (hash << 21) - hash - 1;
		hash = hash ^ (hash >> 24);
		hash = (hash + (hash << 3)) + (hash << 8); // hash * 265
		hash = hash ^ (hash >> 14);
		hash = (hash + (hash << 2)) + (hash << 4); // hash * 21
		hash = hash ^ (hash >> 28);
		hash = hash + (hash << 31);
		return hash;
	}
	static inline uint32 HashDownTo32(uint64 inValue)
	{
		return GetTypeHash( FastHash64(inValue));
	}
	F_INeedA()
	{
	}
	
	struct FeedMap
	{

		std::thread::id That = std::thread::id();
		TSharedPtr<ThreadFeed> Queue = nullptr;

		FeedMap()
		{
			That = std::thread::id();
			Queue = nullptr;
		}

		FeedMap(std::thread::id MappedThread, uint16 MaxQueueDepth)
		{
			That = MappedThread;
			Queue = MakeShareable(new ThreadFeed(MaxQueueDepth));
		}
	};
	
	struct GameFeedMap
	{

		std::thread::id That = std::thread::id();
		TSharedPtr<GameThreadRequestQ> Queue = nullptr;

		GameFeedMap()
		{
			That = std::thread::id();
			Queue = nullptr;
		}

		GameFeedMap(std::thread::id MappedThread, uint16 MaxQueueDepth)
		{
			That = MappedThread;
			Queue = MakeShareable(new GameThreadRequestQ(MaxQueueDepth));
		}
	};

	FeedMap BusyWorkerAcc[ALLOWED_THREADS_FOR_ARTILLERY];
	GameFeedMap GameThreadAcc[ALLOWED_THREADS_FOR_ARTILLERY];
	FeedMap AIThreadAcc[ALLOWED_THREADS_FOR_ARTILLERY];
	uint8 ThreadAccTicker = 0;
	mutable FCriticalSection GrowOnlyAccLock;
	
	void Feed()
	{
		FScopeLock GrantFeedLock(&GrowOnlyAccLock);
		
		//TODO: expand if we need for rollback powers. could be sliiiick
		BusyWorkerAcc[ThreadAccTicker] = FeedMap(std::this_thread::get_id(), 2048);
		GameThreadAcc[ThreadAccTicker] = GameFeedMap(std::this_thread::get_id(), 2048);
		AIThreadAcc[ThreadAccTicker] = FeedMap(std::this_thread::get_id(), 2048);
		MyARTILLERYIndex = ThreadAccTicker;
		++ThreadAccTicker;
	}

	// Gun Handling
	// This requests a new gun by name which will then be bound by attribute key to skeletonkey provided.
	// When your get attrib for the relationship type returns something usable, it can be used.
	// TODO: Get this off the fucking main thread or we'll never be deterministic.
	FGrantWith NewUnboundGun(FSkeletonKey Self, FGunKey NameSetIDUnset,  FARelatedBy EquippedAs, ArtilleryTime Stamp)
	{
		auto MyRequest = FRequestGameThreadThing(ArtilleryRequestType::GetAnUnboundGun);
		MyRequest.Stamp = Stamp;
		MyRequest.SourceOrSelf = Self;
		MyRequest.Gun = NameSetIDUnset;
		MyRequest.Relationship = EquippedAs;
		GameThreadAcc[MyARTILLERYIndex].Queue->Enqueue(MyRequest);
		return FGrantWith(Stamp).Set(FGrantWith::Eventual | FGrantWith::Bound | FGrantWith::GameThread);
	};
	
	FGrantWith NewAutoGun()
	{
		throw; //not implemented yet
	};

	FGrantWith Harvester()
	{
		throw; //not implemented yet
	};

	FGrantWith MobileAI(FSkeletonKey AIEntity, ArtilleryTime Stamp)
	{
		auto MyRequest = FRequestThing(ArtilleryRequestType::BindAI);
		MyRequest.Stamp = Stamp;
		MyRequest.SourceOrSelf = AIEntity;
		AIThreadAcc[MyARTILLERYIndex].Queue->Enqueue(MyRequest);
		return FGrantWith(Stamp).Set(FGrantWith::Eventual | FGrantWith::Within1Tick);
	}
	//the following statement must return a non-null element
	//GunToFiringFunctionMapping->Find(Request.Gun)->ExecuteIfBound(GunByKey->FindRef(Request.Gun), false);
	//in other words, it MUST be bound already as per any other gun you wish to fire. globspeebcormbrad
	FGrantWith GunFired(FGunKey Target, ArtilleryTime Stamp)
	{
		if(this)
		{
			///////////////////
			//build request
			//////////////////
			auto MyRequest = FRequestGameThreadThing(ArtilleryRequestType::FireAGun);
			MyRequest.Stamp = Stamp;
			MyRequest.Gun = Target;
			GameThreadAcc[MyARTILLERYIndex].Queue->Enqueue(MyRequest);
			return FGrantWith(Stamp).Set(FGrantWith::Eventual | FGrantWith::Within1Tick);
		}
		return FGrantWith(Stamp).Set(FGrantWith::Eventual | FGrantWith::GameThread);
	}
	
	//this will just create a lil ticklite that has the number of ticks as its duration, and fires the gun on expire
	FGrantWith GunFiredAtTime(FGunKey Target, ArtilleryTime Stamp)
	{
		if(this)
		{
			///////////////////
			//build request
			//////////////////
			auto MyRequest = FRequestGameThreadThing(ArtilleryRequestType::CreateATicklite);
			MyRequest.Stamp = Stamp;
			MyRequest.Gun = Target;
			GameThreadAcc[MyARTILLERYIndex].Queue->Enqueue(MyRequest);
			return FGrantWith(Stamp).Set(FGrantWith::Eventual | FGrantWith::Within1Tick);
		}
		return FGrantWith(Stamp).Set(FGrantWith::Nullable);
	};

	//this will just create a lil ticklite that has the number of ticks as its duration, and fires the gun on expire
	FGrantWith GunFiredFromATicklite(FRequestGameThreadThing FireMeElmo)
	{
		throw; //not implemented yet
	};
	
	FGrantWith GunFiredWhenATagGetsAdded()
	{
		throw; //not implemented yet
	};
	
	FGrantWith GunFiredWhenATagExpires()
	{
		throw; //not implemented yet
	};
	
	bool AutoGunTurnedOff(FGunKey Target)
	{
		throw;
	}
	bool AutoGunTurnedOn(FGunKey Target)
	{
		throw; //not implemented yet
	};

	// Particle Systems
	FGrantWith ParticleSystemActivatedOrDeactivated(FParticleID PID, bool ShouldBeActive, ArtilleryTime Stamp)
	{
		if (this)
		{
			auto MyRequest = FRequestGameThreadThing(ArtilleryRequestType::ParticleSystemActivateOrDeactivate);
			// Munging the uint32 we use for a Particle ID into the uint64 we use for a skeleton key should be safe
			MyRequest.SourceOrSelf = PID.ParticleId;
			MyRequest.Stamp = Stamp;
			MyRequest.ActivateIfPossible = ShouldBeActive;
			GameThreadAcc[MyARTILLERYIndex].Queue->Enqueue(MyRequest);
			return FGrantWith(Stamp).Set(FGrantWith::Eventual | FGrantWith::Within1Tick);
		}
		return FGrantWith(Stamp).Set(FGrantWith::Nullable);
	}
	
	FGrantWith ParticleSystemSpawnedAttached(FName ThingName, const FSkeletonKey& ComponentToAttachTo, const FSkeletonKey& Owner, ArtilleryTime Stamp, bool CreateSceneComponentOnKey = false)
	{
		if (this)
		{
			auto MyRequest = FRequestGameThreadThing(ArtilleryRequestType::SpawnParticleSystemAttached);
			MyRequest.ThingName = ThingName;
			MyRequest.Stamp = Stamp;
			MyRequest.SourceOrSelf = ComponentToAttachTo;
			MyRequest.TargetOrNonSelfAffected = Owner;
			MyRequest.ActivateIfPossible = CreateSceneComponentOnKey;
			GameThreadAcc[MyARTILLERYIndex].Queue->Enqueue(MyRequest);
			return FGrantWith(Stamp).Set(FGrantWith::Eventual | FGrantWith::Within1Tick);
		}
		return FGrantWith(Stamp).Set(FGrantWith::Nullable);
	}

	FGrantWith ParticleSystemSpawnAtLocation(FName ThingName, const FVector& Location, const FRotator& Rotation, ArtilleryTime Stamp)
	{
		if (this)
		{
			auto MyRequest = FRequestGameThreadThing(ArtilleryRequestType::SpawnParticleSystemAtLocation);
			MyRequest.ThingName = ThingName;
			MyRequest.Stamp = Stamp;
			MyRequest.ThingVector = Location;
			MyRequest.ThingRotator = Rotation;
			GameThreadAcc[MyARTILLERYIndex].Queue->Enqueue(MyRequest);
			return FGrantWith(Stamp).Set(FGrantWith::Eventual | FGrantWith::Within1Tick);
		}
		return FGrantWith(Stamp).Set(FGrantWith::Nullable);
	}

	// Mesh Creation AND Barrage object creation. Later, this will be split for better sim rollback and faster sim responsivity.
	// The game thread should pretty much never directly be responsible for creating barrage objects, its timing is too variable.
	// -1 ticks is default
	FGrantWith Bullet(FName ThingName, FVector Location, double Scale, FVector StartingVelocity, FSkeletonKey NewProjectileKey, const FGunKey& Gun, ArtilleryTime Stamp, Layers::EJoltPhysicsLayer Layer = Layers::PROJECTILE, int LifeInTicks = -1, bool HasExpiration = true)
	{
		if (this)
		{
			auto MyRequest = FRequestGameThreadThing(ArtilleryRequestType::SpawnStaticMesh);
			MyRequest.ThingName = ThingName;
			MyRequest.Stamp = Stamp;
			MyRequest.ThingVector = Location;			// Start Location
			MyRequest.ThingVector2.X = Scale;			// Spawn Scale
			MyRequest.ThingVector3 = StartingVelocity;	// Spawn Velocity
			MyRequest.SourceOrSelf = NewProjectileKey;
			MyRequest.Gun = Gun;
			MyRequest.Layer = Layer;
			MyRequest.CanExpire = true;
			MyRequest.TicksDuration = LifeInTicks;
			GameThreadAcc[MyARTILLERYIndex].Queue->Enqueue(MyRequest);
			return FGrantWith(Stamp).Set(FGrantWith::Eventual | FGrantWith::Within1Tick);
		}
		return FGrantWith(Stamp).Set(FGrantWith::Nullable);
	}
};


