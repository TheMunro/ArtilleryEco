#pragma once

#include "CoreMinimal.h"
#include "ArtilleryActorControllerConcepts.h"
#include "HAL/Runnable.h"
#include "CanonicalInputStreamECS.h"
#include "BristleconeCommonTypes.h"
#include "Containers/TripleBuffer.h"
#include "LocomotionParams.h"

#include "BarrageDispatch.h"
#include "NeedA.h"

//this is a busy-style thread, which runs preset bodies of work in a specified order. Generally, the goal is that it never
//actually sleeps. In fact, it yields rather than sleeps, in general operation.
// 
// This is similar to but functionally very different from a work-stealing or task model like what we see in rust.
// 
// 
// The artilleryworker needs to be kept fairly busy or it will melt one cpu core yield-cycling. to be honest, worth it.
// no, seriously. with all the other sacrifices we've made occupying one core with game-sim physics, reconciliation,
// rollbacks, and pattern matching is a pretty good bargain. we'll want to revisit this for servers, of course.

class FArtilleryBusyWorker : public FRunnable {
	public:
	FArtilleryBusyWorker();
	virtual ~FArtilleryBusyWorker() override;
	//This isn't super safe, but Busy Worker is used in exactly one place
	//and the dispatcher that owns this memory MUST manage this lifecycle.
	TSharedPtr<BufferedMoveEvents>  Locomos_BufferNotThreadSafe;
	TSharedPtr<BufferedEvents> RequestorQueue_Abilities_TripleBuffer;
	TSharedPtr<BufferedAIMoveEvents> RequestorQueue_AI_Locomos_TripleBuffer;
	ArtilleryTime TickliteNow = 0;
	FSharedEventRef StartTicklitesSim;
	FSharedEventRef StartTicklitesApply;
	FSharedEventRef StartRunAhead;
	int SeqNumber = 0;
	//Going forward, it is potentially worthwhile for us switch to this...
	ITickHeavy* ParticleSystemPointer;
	ITickHeavy* ProjectileSystemPointer;
	//Thanks to OrdIn, we can guarantee what is up when, and by expanding OrdIn, we can control what is
	//deinitialized when if we need to, as well. It wouldn't even be that hard, simply add a deregister to SkeletonLord
	
	virtual bool Init() override;
	void RunStandardFrameSim(bool& missedPrior,
		uint64_t& currentIndexCabling,
		bool& burstDropDetected,
		TheCone::PacketElement& current,
		bool& RemoteInput) const;
	void ProcessRequestRouterBusyWorkerThread();
	virtual uint32 Run() override;
	virtual void Exit() override;
	virtual void Stop() override;
	// stop me if you've heard this one before
	
	//this is a hack and MIGHT be replaced with an ECS lookup
	//though the clarity gain is quite nice, and privileging Cabling makes sense
	TSharedPtr<ArtilleryControlStream>  CablingControlStream;
	TSharedPtr<ArtilleryControlStream> BristleconeControlStream;
	TSharedPtr<ArtilleryControlStream> ThistleControlStream;
	TSharedPtr<F_INeedA> RequestRouter;
	TheCone::RecvQueue InputRingBuffer;
	TheCone::SendQueue InputSwapSlot;
	UCanonicalInputStreamECS* ContingentInputECSLinkage;
	UBarrageDispatch* ContingentPhysicsLinkage;
	
private:
	void Cleanup();
	bool running;
};
