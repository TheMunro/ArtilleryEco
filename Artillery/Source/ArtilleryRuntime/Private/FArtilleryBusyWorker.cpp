#include "FArtilleryBusyWorker.h"
#include "ArtilleryDispatch.h"
#include "Windows/WindowsSystemIncludes.h"
#include <timeapi.h>

#include "ArtilleryBPLibs.h"
#include "BarrageDispatch.h"
#include "Containers/TripleBuffer.h"

FArtilleryBusyWorker::FArtilleryBusyWorker() : RequestorQueue_Abilities_TripleBuffer(nullptr), running(false)
{
	UE_LOG(LogTemp, Display, TEXT("Artillery:BusyWorker: Constructing Artillery"));
}

FArtilleryBusyWorker::~FArtilleryBusyWorker()
{
	UE_LOG(LogTemp, Display, TEXT("Artillery:BusyWorker: Destructing Artillery"));
}

bool FArtilleryBusyWorker::Init()
{
	//you cannot reorder these. it is a magic ordering put in place for a hack. 

	UE_LOG(LogTemp, Display, TEXT("Artillery:BusyWorker: Initializing Artillery thread"));
	running = true;
	return true;
}

void FArtilleryBusyWorker::RunStandardFrameSim(bool& missedPrior, uint64_t& currentIndexCabling,
                                               bool& burstDropDetected, PacketElement& current,
                                               bool& RemoteInput)
{
	//this is an odd thing to do, I know, but we have some book-keeping we want to reserve for each code path.
	//once this settles a little, I'll refactor, but I'm going to end up reworking this next weekend.
	if (InputRingBuffer != nullptr && !InputRingBuffer.Get()->IsEmpty())
	{
		while (InputRingBuffer != nullptr && !InputRingBuffer.Get()->IsEmpty())
		{
			const Packet_tpl* packedInput = InputRingBuffer.Get()->Peek();
			const long indexInput = packedInput->GetCycleMeta() + 3; //faster than 3xabs or a branch.
			//unlike the old design, we use an array of inputs from first -> current
			//so we want to add oldest first, then next, then next.
			//we'll need to amend this to handle correct defaulting of missing input,
			//which we can detect by both cycle skips and arrival window misses.
			//we then need a way, during rollbacks, to perform the rewrite.
			//right now, we just wait until we get the remote input.
			if (missedPrior)
			{
				if (burstDropDetected)
				{
					BristleconeControlStream->Add(
						*const_cast<Packet_tpl*>(packedInput)->GetPointerToElement((indexInput - 2) % 3),
						packedInput->GetTransferTime());
				}
					
				BristleconeControlStream->Add(
					*const_cast<Packet_tpl*>(packedInput)->GetPointerToElement((indexInput - 1) % 3),
					packedInput->GetTransferTime());
			}
			BristleconeControlStream->Add(
				*const_cast<Packet_tpl*>(packedInput)->GetPointerToElement(indexInput % 3),
				packedInput->GetTransferTime());

			RemoteInput = true; //we check for empty at the start of the while. no need to check again.
			InputRingBuffer.Get()->Dequeue();
		}

		if (RemoteInput == true)
		{
			missedPrior = false;
			burstDropDetected = false;
		}
		else
		{
			if (burstDropDetected)
			{
				//add rolling average switch-over here
			}
			if (missedPrior)
			{
				burstDropDetected = true;
			}
			missedPrior = true;
		}
	}
	else if (InputSwapSlot != nullptr && !InputSwapSlot.Get()->IsEmpty())
	{
		//though it's probably more elegant and faster to index over the control streams
		while (InputSwapSlot != nullptr && !InputSwapSlot.Get()->IsEmpty())
		{
			current = *InputSwapSlot.Get()->Peek();
			CablingControlStream->Add(current);

			InputSwapSlot.Get()->Dequeue();
		}
	}
	else
	{
		//----------------------------------
		//if we got nothing, repeat prior.
		//0000000000000000000000000000000000

		CablingControlStream->Add(CablingControlStream->get(CablingControlStream->highestInput - 1)->MyInputActions,
		                          TickliteNow);
	}
#define ARTILLERY_FIRE_CONTROL_MACHINE_HANDLING (false)
	//First, locomotions are pushed. Patterns run here. The thread queues the locomotions and fires.
	//the dispatch fires guns via the machines on the gamethread.

	//Pattern matchers match, set events, and then those events are handed to the dispatch for now.
	//gradually, we'll be able to run more and more of them on this thread, freeing us from the tyranny.
	//Per input stream, run their patterns here. god in heaven.
	EventBuffer& refDangerous_LifeCycleManaged_Abilities_TripleBuffered = RequestorQueue_Abilities_TripleBuffer->GetWriteBuffer();

	if (currentIndexCabling < CablingControlStream->highestInput)
	{
		//today's sin is PRIDE, bigbird!
		for (int i = currentIndexCabling; i < CablingControlStream->highestInput; ++i)
		{
			//TODO: does this leak memory?
			ActorKey StreamActorKey = CablingControlStream->GetActorByInputStream();
			if (StreamActorKey)
			{
				Locomos_BufferNotThreadSafe->Add(
					LocomotionParams(
						CablingControlStream->peek(i)->SentAt,
						StreamActorKey,
						*CablingControlStream->peek(i - 1),
						*CablingControlStream->peek(i)));

				// this looks wrong but I'm pretty sure it ain' since we reserve highest.
				CablingControlStream->MyPatternMatcher->runOneFrameWithSideEffects(
					true,
					0,
					0,
					i,
					refDangerous_LifeCycleManaged_Abilities_TripleBuffered); 
			}
			//even if this doesn't get played for some reason, this is the last chance we've got to make a
			//truly informed decision about the matter. By the time we reach the dispatch system, that chance is gone.
			//Better to skip a cosmetic once in a while than crash the game.
			CablingControlStream->get(CablingControlStream->highestInput - 1)->RunAtLeastOnce = true;
		}
	}

	Locomos_BufferNotThreadSafe->Sort();
	refDangerous_LifeCycleManaged_Abilities_TripleBuffered.Sort();
	if (RequestorQueue_Abilities_TripleBuffer->IsDirty() == false)
	{
		RequestorQueue_Abilities_TripleBuffer->SwapWriteBuffers();
	}
}

//TODO right now, this incurs two serious determinism risks:
//The order that threads get queues is random, so if you just go down the line, that won't produce a deterministic execution order.
//Even if you fix that, you still need to order the requests as a gestalt, and now you have a problem where you don't know the
//correct\true order to run things with the same timestamp in. This is fixable but it's gonna need to wait.
void FArtilleryBusyWorker::ProcessRequestRouterBusyWorkerThread()
{
	if (RequestRouter)
	{
		for (F_INeedA::FeedMap& WorkerFeedMap : RequestRouter->BusyWorkerAcc)
		{
			TSharedPtr<F_INeedA::ThreadFeed> HoldOpen;
			if (WorkerFeedMap.Queue && ((HoldOpen = WorkerFeedMap.Queue)) && WorkerFeedMap.That != std::thread::id()) //if there IS a thread.
			{
				FRequestThing RouterQueue;
				while (HoldOpen->Dequeue(RouterQueue))
				{
					//PINPOINT: YABUSYTHREADBOYRUNNETHREQUESTSHERE

				}
			}
		}
	}
}

uint32 FArtilleryBusyWorker::Run()
{
	UE_LOG(LogTemp, Display, TEXT("Artillery:BusyWorker: Running Artillery thread"));
	if (RequestorQueue_Abilities_TripleBuffer == nullptr)
	{
#ifdef UE_BUILD_SHIPPING
		return -1;
#else
		throw; // this is a BUG. A BAD ONE. 
#endif
	}
	
	bool missedPrior = false;
	uint64_t currentIndexCabling = 0;
	bool burstDropDetected = false;
	bool sent = false;
	//TODO: remember why this needs to be an int. 
	//if you wanna use this for a really long lived session, you'll need to fix it. you know. one longer than 34 years.
	SeqNumber = 0;
	//Hi! Jake here! Reminding you that this will CYCLE
	//That's known. Isn't that fun? :) Don't reorder these, by the way.
	uint32_t LastIncrementWindow = ContingentInputECSLinkage->Now();
	uint32_t lsbTime = ContingentInputECSLinkage->Now();
	constexpr uint32_t sampleHertz = TheCone::CablingSampleHertz;
	constexpr uint32_t RunHertz = LongboySendHertz;
	const uint32_t SendHertzFactor = sampleHertz / RunHertz; // THIS ROUNDS DOWN. IT IS INT MATH.
	//in other words, artillery will always run at powers of two right now. that's intended for prototype.
	constexpr uint32_t Period = 999900 / sampleHertz;
	//swap to microseconds. standardizing. we actually run a LITTLE fast. for science reasons.
	constexpr double MarginOfErrorForSleep = 0.75;
	constexpr double MarginOfErrorForSpin = 0.90; //roughly .2 ms. Nothing but a spin is gonna be fast enough.
	
	//sleep basically always has +/1 duration * .5 error, at our sample rates.
	constexpr uint32_t ErrorAdjustedPeriod = Period * MarginOfErrorForSleep;
	
	constexpr uint32_t MicroErrorAdjustedPeriod = Period * MarginOfErrorForSpin;
	// we prefer to land near the _start_ of a period, so we bias.

	constexpr std::chrono::milliseconds Step(Period / 1000);

	//we can now start the sim. we latch only on the apply step.
	StartTicklitesSim->Trigger();
	//we are started by Artillery Dispatch, but we can't use it in the .h file to avoid dependencies.
	//so we know it's live, but we don't take a ref to it until this point.
	//we only use it for GrantFeed, but it's important that we start abiding by separation of concerns
	//where we can, so we're trying to hide the barrage dependency here in a sense. We can't fully, but.
	UArtilleryDispatch* ArtilleryDispatch = ContingentInputECSLinkage->GetWorld()->GetSubsystem<UArtilleryDispatch>();
	ArtilleryDispatch->ThreadSetup();
	timeBeginPeriod(2);
	
	while (running)
	{
		if (!sent &&
			(
				InputRingBuffer != nullptr && !InputRingBuffer.Get()->IsEmpty()
				|| SeqNumber % SendHertzFactor <= SendHertzFactor / 2 //we no longer slide all the way to the end.
				//Instead, we only slide to 1/2, so somewhat less latency than a curious version of 256hz.
				//I resent it, but the beast of nonetime had to go. 
			)
		)
		{
			currentIndexCabling = CablingControlStream->highestInput;
			PacketElement current = 0;
			bool RemoteInput = false;
			RunStandardFrameSim(missedPrior, currentIndexCabling, burstDropDetected, current, RemoteInput);
			/*
			* Note: We also have Iris performing intermittent state stomps to recover from more serious desyncs.
			* Ultimately, rollback can never solve everything. The windows just get too wide.
			*/
			sent = true;
			TickliteNow = ContingentInputECSLinkage->Now(); // this updates ONCE PER CYCLE. ONCE. THIS IS INTENDED.

			ProcessRequestRouterBusyWorkerThread();
			ArtilleryDispatch->RunLocomotions();
			//such a simple thing, after all this work.
			if (ContingentPhysicsLinkage == nullptr) 
			{
				//TODO: do we need to trigger this here????
				//StartTicklitesApply->Trigger();
			}
			else // yeah, I know it's optional, but stylistically, it's important.
			{
				ContingentPhysicsLinkage->StackUp();
				StartTicklitesApply->Trigger();
				StartRunAhead->Trigger();
				ContingentPhysicsLinkage->StepWorld(TickliteNow, SeqNumber);
				ContingentPhysicsLinkage->BroadcastContactEvents();
				if (ParticleSystemPointer)
				{
					ParticleSystemPointer->ArtilleryTick(); //currently a no-op.
				}
				if (ProjectileSystemPointer)
				{
					ProjectileSystemPointer->ArtilleryTick();
				}
			}
		}

		//unlike cabling, we do our time keeping HERE. It may be worth switching cabling to also follow this.
		//Alternatively, it may be worthwhile to switch to a wait/wake pattern against cabling, where we wait half the interval max, then
		//start simulating in the remaining half. We're already eating 4ms of latency, and the act of sleeping is getting
		//kind of expensive actually.
		if (LastIncrementWindow + Period <= lsbTime)
		{
			LastIncrementWindow = FMath::Min(LastIncrementWindow + Period, lsbTime);
			if (SeqNumber % SendHertzFactor == 0)
			{
				sent = false;
			}
			++SeqNumber;
		}
		else if ((LastIncrementWindow + MicroErrorAdjustedPeriod) <= lsbTime)
		{
			//spin spin spin
			
		}
		else if ((LastIncrementWindow + ErrorAdjustedPeriod) <= lsbTime)
		{
			std::this_thread::yield(); //if we're within the margin of error on sleep, we yield instead.
		}
		else
		{
			std::this_thread::sleep_for(Step);
		}
		//std::this_thread::yield();
		lsbTime = ContingentInputECSLinkage->Now();
		if ((SeqNumber % sampleHertz) == 0)
		{
			UE_LOG(LogTemp, Display, TEXT("Artillery Busy Worker hertz cycled: %u against seq %ld"),
				   lsbTime, SeqNumber);
		}
	}
	
	timeEndPeriod(2);
	UE_LOG(LogTemp, Display, TEXT("Artillery:BusyWorker: Run Ended."));
	return 0;
}

void FArtilleryBusyWorker::Exit()
{
	UE_LOG(LogTemp, Display, TEXT("ARTILLERY OFFLINE."));
	Cleanup();
}

void FArtilleryBusyWorker::Stop()
{
	UE_LOG(LogTemp, Display, TEXT("Artillery:BusyWorker: Stopping Artillery thread."));
	Cleanup();
}

void FArtilleryBusyWorker::Cleanup()
{
	running = false;
}
