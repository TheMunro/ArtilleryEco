// Jolt Physics Library
// Barrage Extension

#include "HungryBroadPhase.h"
#include <concepts>
#include "Jolt/Geometry/ClosestPoint.h"


JPH_NAMESPACE_BEGIN
	//shadow copy ops are a tick apart. as a result, you need to be not one, but two ticks behind to actually experience repercussions.
	//this means we _directly reference without locking_
	void HungryBroadPhase::CastRay(const RayCast& inRay, RayCastBodyCollector& ioCollector,
	                               const BroadPhaseLayerFilter& inBroadPhaseLayerFilter,
	                               const ObjectLayerFilter& inObjectLayerFilter, uint32_t AllowedHits) const
	{
		// Load ray
		Vec3 origin(inRay.mOrigin);
		RayInvDirection inv_direction(inRay.mDirection);
		thread_local std::vector<BodyID> Results;
		Results.clear();
		//highway to the dangerzonnnneeeeee
		//auto RayQueryResult = RTree->rayQuery(origin.mF32, origin.mF32, std::back_inserter(Results));
		// For all bodies
		float early_out_fraction = ioCollector.GetEarlyOutFraction();
		uint32_t hits = 0;
		ApproxCastRay(inRay, ioCollector, inBroadPhaseLayerFilter, inObjectLayerFilter, AllowedHits);
	}

	bool HungryBroadPhase::ScoopUpHitsRay(RayCastBodyCollector& ioCollector,
	                                      const ObjectLayerFilter& inObjectLayerFilter,
	                                      uint32_t AllowedHits, Vec3 origin, RayInvDirection inv_direction,
	                                      std::vector<uint32_t> tset, float early_out_fraction, uint32_t hits) const
	{
		size_t prevID = SIZE_T_MAX;
		//we actually get a ton of info from WHICH points in a BB's set are in our knn.
		//using it is a bit complicated, and I just want to see if this works at all first.
		//but if you're reading this, just be aware that there's a ton of optimizations available to us using a truthtable
		//right now, tset isn't even necessarily sorted from my grasp on the problem, not even really by distance due to the approximating
		//behavior. one interesting thing, though, is that this is a COVERING LSH so because we're pulling so many points, we will always get
		//at least some of the actual nearest neighbor as far as I can deduce.
		for (auto b : tset)
		{
			auto idx = b;
			if (idx == prevID)
			{
				//this will glide us past the other points from a bb's set that we hit
				//BEFORE we dig up the body id and grab the body.
				continue;
			}
			prevID = idx;
			//turn the idx into the body id by key_ref
			auto id = BodyID((*mBodies)[idx].meta);
			//TODO: check the api... I think this throws on a missed find...

			//get the original bb. doing it this way may mean that we can release the ds_ref? idk yet.
			auto& bodyshadow = (*mBodies)[idx]; //body shadows are both smaller and safer.

			// Test layer
			if (
				inObjectLayerFilter.ShouldCollide(bodyshadow.GetObjectLayer())
				&& hits < AllowedHits)
			{
				// Test intersection with ray
				const AABox& bounds = bodyshadow.GetWorldSpaceBounds();
				float fraction = RayAABox(origin, inv_direction, bounds.mMin, bounds.mMax);
				if (fraction < early_out_fraction)
				{
					// Store hit
					BroadPhaseCastResult result{id, fraction};
					ioCollector.AddHit(result);
					if (ioCollector.ShouldEarlyOut())
					{
						return true;
					} //we're don!
					early_out_fraction = ioCollector.GetEarlyOutFraction();
				}
			}
		}
		return false;
	}

	void HungryBroadPhase::ApproxCastRay(const RayCast& inRay, RayCastBodyCollector& ioCollector,
	                                     const BroadPhaseLayerFilter& inBroadPhaseLayerFilter,
	                                     const ObjectLayerFilter& inObjectLayerFilter, uint32_t AllowedHits) const
	{
		// Load ray
		RayInvDirection inv_direction(inRay.mDirection);

		//as you read this, bear in mind that we give even points a minimum volume. hbp only deals with positive volume convex solids represented as AABBs.
		thread_local std::vector<BodyID> Results;
		Results.clear();
		//highway to the dangerzonnnneeeeee
		//isn't this fun? it was easiest to fully decompose the damn thing instead of screwing with pack pragmas
		// and four different vec types.
		FLESHPoint Query = FLESHPoint::FromLine(
			inRay.mOrigin.GetX(), inRay.mOrigin.GetY(), inRay.mOrigin.GetZ(),
			inRay.mDirection.GetX(), inRay.mDirection.GetY(), inRay.mDirection.GetZ());

		// For all bodies
		float early_out_fraction = ioCollector.GetEarlyOutFraction();
		uint32_t hits = 0;
		EMBED::HashBlob& hashes = EMBED::getHash(Query, 1);
		std::vector<uint32_t> results = LSH->query(hashes,  PointsPerBB * 2);

		//idk why this is here....
		ScoopUpHitsRay(ioCollector, inObjectLayerFilter, AllowedHits, inRay.mOrigin, inv_direction, results,
		               early_out_fraction,
		               hits);
	}

	void HungryBroadPhase::CollideAABox(const AABox& inBox, CollideShapeBodyCollector& ioCollector,
	                                    const BroadPhaseLayerFilter& inBroadPhaseLayerFilter,
	                                    const ObjectLayerFilter& inObjectLayerFilter) const
	{
	struct AABBCollideBehavior
	{
		CollideShapeBodyCollector& ioCollector;

		AABBCollideBehavior(CollideShapeBodyCollector& ioCollector): ioCollector(ioCollector)
		{
		}

		bool Acc(const AABox& inBox,
				 const ObjectLayerFilter& inObjectLayerFilter, float lenny,
				 [[maybe_unused]] UnorderedSet<uint32_t>& Setti, HungryBroadPhase::BodyBoxSafeShadow& body,
				 [[maybe_unused]] uint32_t idx)
		{
			if (inBox.Intersect(body.GetWorldSpaceBounds()).IsValid())
			{
				if (inObjectLayerFilter.ShouldCollide(body.layer))
				{
					ioCollector.AddHit(body.meta);
					if (ioCollector.ShouldEarlyOut())
					{
						return true;
					}
				}
			}
			return false;
		}
	};
	auto Pin = mBodies;
	thread_local UnorderedSet<uint32_t> Setti;
	Setti.reserve(150);
		std::vector<FLESHPoint> Batch(9);
		//we do this SO much less than I expected.
		auto a = inBox.GetCenter();
		Batch[0]
			= FLESHPoint::FromPoint(a.GetX(), a.GetY(), a.GetZ(), MaxExtent);

		//auto min
		Batch[1]
			= FLESHPoint::FromPoint((inBox.mMin.GetX()), (inBox.mMin.GetY()), (inBox.mMin.GetZ()), MaxExtent);
		//auto max
		Batch[2]
			= FLESHPoint::FromPoint(inBox.mMax.GetX(), inBox.mMax.GetY(), inBox.mMax.GetZ(), MaxExtent);
		//auto x
		Batch[3]
			= FLESHPoint::FromPoint(inBox.mMax.GetX(), (inBox.mMin.GetY()), (inBox.mMin.GetZ()), MaxExtent);
		//auto xy
		Batch[4]
			= FLESHPoint::FromPoint(inBox.mMax.GetX(), (inBox.mMax.GetY()), (inBox.mMin.GetZ()), MaxExtent);
		//auto xz
		Batch[5]
			= FLESHPoint::FromPoint(inBox.mMax.GetX(), (inBox.mMin.GetY()), (inBox.mMax.GetZ()), MaxExtent);
		//auto y
		Batch[6]
			= FLESHPoint::FromPoint(inBox.mMin.GetX(), (inBox.mMax.GetY()), (inBox.mMin.GetZ()), MaxExtent);
		//auto yz
		Batch[7]
			= FLESHPoint::FromPoint(inBox.mMin.GetX(), (inBox.mMax.GetY()), (inBox.mMax.GetZ()), MaxExtent);
		//auto z
		Batch[8]
			= FLESHPoint::FromPoint(inBox.mMin.GetX(), (inBox.mMin.GetY()), (inBox.mMax.GetZ()), MaxExtent);

		auto QHashes = LSH->get9Hashes(Batch, 1);
		auto results = LSH->query(QHashes.data(), 9, 50);
		Accumulate<AABBCollideBehavior, AABox>(inBox,ioCollector, inObjectLayerFilter, 0, Setti, results);
		Setti.ClearAndKeepMemory();
	}

	void HungryBroadPhase::CollideSphere(Vec3Arg inCenter, float inRadius, CollideShapeBodyCollector& ioCollector,
	                                     const BroadPhaseLayerFilter& inBroadPhaseLayerFilter,
	                                     const ObjectLayerFilter& inObjectLayerFilter) const
	{
		struct SphereCollideBehavior
		{
			CollideShapeBodyCollector& ioCollector;

			SphereCollideBehavior(CollideShapeBodyCollector& ioCollector): ioCollector(ioCollector)
			{
			}

			bool Acc(const Vec3& InCent,
			         const ObjectLayerFilter& inObjectLayerFilter, float lenny,
			         [[maybe_unused]] UnorderedSet<uint32_t>& Setti, HungryBroadPhase::BodyBoxSafeShadow& body,
			         [[maybe_unused]] uint32_t idx)
			{
				if (body.GetWorldSpaceBounds().GetSqDistanceTo(InCent) <= lenny*lenny)
				{
					if (inObjectLayerFilter.ShouldCollide(body.layer))
					{
						ioCollector.AddHit(body.meta);
						if (ioCollector.ShouldEarlyOut())
						{
							return true;
						}
					}
				}
				return false;
			}
		};
		auto Pin = mBodies;
		thread_local UnorderedSet<uint32_t> Setti;
		Setti.reserve(150);
		FLESHPoint A = FLESHPoint::FromPoint(inCenter.GetX(), inCenter.GetY(), inCenter.GetZ(), MaxExtent);
		auto result = LSH->query(EMBED::getHash(A, 1), 100);
		Accumulate<SphereCollideBehavior, Vec3>(inCenter, ioCollector, inObjectLayerFilter, inRadius, Setti, result);
		Setti.ClearAndKeepMemory();
	}

	void HungryBroadPhase::CollidePoint(Vec3Arg inPoint, CollideShapeBodyCollector& ioCollector,
	                                    const BroadPhaseLayerFilter& inBroadPhaseLayerFilter,
	                                    const ObjectLayerFilter& inObjectLayerFilter) const
	{
		//points are just very small spheres. we don't do anything smaller than this, because we already voxelize.
		return CollideSphere(inPoint, 1, ioCollector, inBroadPhaseLayerFilter, inObjectLayerFilter);
	}

	//unused and unsupported. beepboop
	void HungryBroadPhase::CollideOrientedBox(const OrientedBox& inBox, CollideShapeBodyCollector& ioCollector,
	                                          const BroadPhaseLayerFilter& inBroadPhaseLayerFilter,
	                                          const ObjectLayerFilter& inObjectLayerFilter) const
	{
		throw;
	}


	template <typename T>
	concept Accumulator = requires(T a) { a.Acc(); };

	//accumulator behaviors effectively determine when to stop seeking results or running. The accumulate function
	//is basically just a higher order function that applies the behavior in a specific way. I just happen to be most comfy with templates in this context.
	template <typename behavior, typename T>
	bool HungryBroadPhase::Accumulate(const T& inBox, behavior collector,
	                                  const ObjectLayerFilter& inObjectLayerFilter, float lenny,
	                                  UnorderedSet<uint32_t>& Setti, std::vector<uint32_t>& result) const
	{
		if (!result.empty())
		{
			for (auto idx : result)
			{
				uint64_t r = idx / PointsPerBB; // this is wrong?
				if (Setti.insert(r).second)
				{
					auto& body = (*mBodiesShadow)[(idx / PointsPerBB)];

					if ((collector.Acc(inBox, inObjectLayerFilter, lenny, Setti, body, idx)))
					{
						return true;
					}
				}
			}
		}
		return false;
	}


	void HungryBroadPhase::CastAABoxNoLock(const AABoxCast& inBox, CastShapeBodyCollector& ioCollector,
	                                       const BroadPhaseLayerFilter& inBroadPhaseLayerFilter,
	                                       const ObjectLayerFilter& inObjectLayerFilter) const
	{
		// Load box
		struct AABBCastBehavior
		{
			CastShapeBodyCollector& ioCollector;

			AABBCastBehavior(CastShapeBodyCollector& ioCollector): ioCollector(ioCollector)
			{
			}

			bool Acc(const AABoxCast& inBox,
			         const ObjectLayerFilter& inObjectLayerFilter, float lenny,
			         UnorderedSet<uint32_t>& Setti, HungryBroadPhase::BodyBoxSafeShadow& body,
			         [[maybe_unused]] uint32_t idx)
			{
				auto closest = inBox.mBox.GetClosestPoint(body.GetCenter());
				auto k = RayAABox(closest, RayInvDirection(inBox.mDirection), {__LOCO_FFABV(body)},
				                  {__LOCO_FFABVM(body)});
				bool hit = lenny / k < 1.0;
				if (hit)
				{
					if (inObjectLayerFilter.ShouldCollide(body.layer))
					{
						ioCollector.AddHit(BroadPhaseCastResult(body.meta, hit));
						if (ioCollector.ShouldEarlyOut())
						{
							Setti.ClearAndKeepMemory();
							return true;
						}
					}
				}
				return false;
			}
		};

		Vec3 origin(inBox.mBox.GetCenter());
		Vec3 mini(inBox.mBox.mMin);
		Vec3 maxi(inBox.mBox.mMax);
		Vec3 extent(inBox.mBox.GetExtent());
		float lenny = extent.Length();
		float early_out_fraction = ioCollector.GetPositiveEarlyOutFraction();
		auto Pin = mBodies;
		thread_local UnorderedSet<uint32_t> Setti;
		Setti.reserve(150);
		std::vector<uint32_t> result(150);
		if (Pin)
		{
			std::vector<FLESHPoint> Aleph;
			Aleph[0] = FLESHPoint::FromLine(origin.GetX(), origin.GetY(), origin.GetZ(), extent.GetX(), extent.GetY(),
			                                extent.GetZ());
			Aleph[1] = FLESHPoint::FromLine(mini.GetX(), mini.GetY(), mini.GetZ(), extent.GetX(), extent.GetY(),
			                                extent.GetZ());
			Aleph[2] = FLESHPoint::FromLine(maxi.GetX(), maxi.GetY(), maxi.GetZ(), extent.GetX(), extent.GetY(),
			                                extent.GetZ());
			auto hashstack = LSH->getHashes(Aleph, 1);
			uint32_t unused = 0;
			LSH->query_byoa(hashstack.data(), 3, 50,  result);
			if (Accumulate<AABBCastBehavior>(inBox, ioCollector, inObjectLayerFilter, lenny, Setti, result))
			{
				Setti.ClearAndKeepMemory();
				return;
			}
		}
		Setti.ClearAndKeepMemory();
	}

	void HungryBroadPhase::FindCollidingPairs(BodyID* ioActiveBodies, int inNumActiveBodies,
	                                          float inSpeculativeContactDistance,
	                                          const ObjectVsBroadPhaseLayerFilter& inObjectVsBroadPhaseLayerFilter,
	                                          const ObjectLayerPairFilter& inObjectLayerPairFilter,
	                                          BodyPairCollector& ioPairCollector) const
	{
		auto Pin = mBodies;
		if (Pin)
		{
			for (auto& BodyShadowStruct : *Pin)
			{
				FLESHPoint Query = FLESHPoint::FromPoint(
					BodyShadowStruct.x, BodyShadowStruct.y, BodyShadowStruct.z, MaxExtent);
				std::vector<uint32_t> result = LSH->query(EMBED::getHash(Query, 1), 50);
				if (!result.empty())
				{
					for (auto clonked : result)
					{
						//again, I think keyref can be shrunk by 9x. I wanna get the main machine running first.
						BodyID hit = (*mBodies)[clonked / 9].meta; //much easier to keep the key_ref set in mem.
						auto& body2 = mBodyManager->GetBody(hit);
						auto& body1 = mBodyManager->GetBody(BodyShadowStruct.meta);
						if (!Body::sFindCollidingPairsCanCollide(body1, body2))
							continue;

						// Check if layers can collide
						const ObjectLayer layer1 = BodyShadowStruct.GetObjectLayer();
						if (!inObjectLayerPairFilter.ShouldCollide(layer1, body2.GetObjectLayer()))
							continue;

						// Check if bounds overlap

						const AABox& bounds2 = body2.GetWorldSpaceBounds();
						if (!bounds2.Overlaps(BodyShadowStruct.GetWorldSpaceBounds()))
							continue;

						// Store overlapping pair
						ioPairCollector.AddHit({hit, BodyShadowStruct.meta});
					}
				}
			}
		}
	}

	void HungryBroadPhase::TestInvokeBuild()
	{
		//NO-OP for the time being!!!
		//UpdateFinalize(UpdateState());
	}

	//FIt-SNE may be of eventual interest but I'm _really_ hoping not. That said, they do something positively
	//fascinating with the fast fourier that may actually point the way to a fast option for lightweight
	//dim reduction
	void HungryBroadPhase::UpdateFinalize(const UpdateState& inUpdateState)
	{
		mBodiesShadow->clear();
		float MaxExtentShadow = 0;
		if (mBodyManager)
		{
			mBodiesShadow->reserve(mBodyManager->GetNumBodies());
			for (auto body : mBodyManager->GetBodies())
			{
				if (mBodyManager->sIsValidBodyPointer(body))
				{
					JPH::BodyID M = body->GetID();
					if (!M.IsInvalid())
					{
						//build mbodies here
						// we expand the bounding box by the speculative contact distance HERE, not during cast or collide.
						AABox bounds1 = body->GetWorldSpaceBounds();
						auto Cent = bounds1.GetCenter();
						auto Extt = bounds1.GetExtent();
						bounds1.ExpandBy(Vec3::sReplicate(SpeculativeContactDistance));
						BodyBoxSafeShadow J(Cent.GetX(), Cent.GetY(), Cent.GetZ(),
						                    Extt.GetX(), Extt.GetY(), Extt.GetZ(),
						                    body->GetObjectLayer(), M);
						mBodiesShadow->push_back(J);
					}
				}
			}
		}
		mBodies.swap(mBodiesShadow);
		//build and swap rtree here
		RebuildLSH();
	}

	//without the RTree, bounds will need to be accumulated for maximum speed.
	//There's not a great way to recover bounds info from the LSH, and we need it anyway
	//for the T-constant used in the point embedding
	AABox HungryBroadPhase::GetBounds() const
	{
		AABox bounds({0, 0, 0}, MaxExtent);
		return bounds;
	}

	void HungryBroadPhase::GenerateBBHashes(float oldM, std::vector<FLESHPoint>& Batch,
	                                        std::vector<HungryBroadPhase::BodyBoxSafeShadow>::value_type& body)
	{
		Batch[0]
			= FLESHPoint::FromPoint(__LOCO_FFABVC(body), oldM);

		//auto min
		Batch[1]
			= FLESHPoint::FromPoint(__LOCO_FFABV(body), oldM);
		//auto max
		Batch[2]
			= FLESHPoint::FromPoint(__LOCO_FFABVM(body), oldM);
		//auto x
		Batch[3]
			= FLESHPoint::FromPoint(__LOCO_FFABVX(body), oldM);
		//auto xy
		Batch[4]
			= FLESHPoint::FromPoint(__LOCO_FFABVXY(body), oldM);
		//auto xz
		Batch[5]
			= FLESHPoint::FromPoint(__LOCO_FFABVXZ(body), oldM);
		//auto y
		Batch[6]
			= FLESHPoint::FromPoint(__LOCO_FFABVY(body), oldM);
		//auto yz
		Batch[7]
			= FLESHPoint::FromPoint(__LOCO_FFABVYZ(body), oldM);
		//auto z
		Batch[8]
			= FLESHPoint::FromPoint(__LOCO_FFABVZ(body), oldM);
		//preallocing this or doing it in one massive sweep would be FAR faster.
		//but let's see if we need to optimize seriously first. I fear we will need to, quite badly.
		//because we generate 9 points per point, we actually do... some evil shit here as mentioned above.
		//namely, mBodies is what our CLSHoid refers into AND we specialize our hasher.
	}

	void HungryBroadPhase::RebuildLSH()
	{
		//////////////////////////
		///UNDER CONSTRUCTION
		////////
		///it really looks like we can use the index mappings behavior of FLINNG to avoid ever
		/// actually storing our embeddings. That'd be EXTREMELY dope, bluntly.

		float oldM = MaxExtent;
		MaxExtent = 0;
		LSHShadow->RegenerateTheFLESH(mBodies->size() * PointsPerBB);
		//if an ordering is applied to mBodies, you'll get a deterministic build of the LSH.
		std::vector<FLESHPoint> Batch(9);
		for (auto& body : *mBodies)
		{
			auto id = body.meta;
			//you might notice that we push the center in first! that means it's always the lowest idx of a bb's point set.
			//that's semantically important. please don't "fix" this.

			//////////////////////////////////////////////////////////////
			///WARNING: SUBTLE AND EVIL
			//PointsPerBB must be adjusted as this code changes. failure to do so will cause funny but devastating bugs.
			//I believe we can reduce the number of datastructures here significantly as we transisition away from CLSH
			//////////////////////////////////////////////////////////////
			//auto ctr
			auto c1Extent = abs(FVector::Distance(FVector::ZeroVector, {__LOCO_FFABVM(body)}));
			auto c2Extent = abs(FVector::Distance(FVector::ZeroVector, {__LOCO_FFABVM(body)}));
			MaxExtent = ceil(FMath::Max3(MaxExtent, c1Extent, c2Extent));
			GenerateBBHashes(oldM, Batch, body);
			LSHShadow->addHashedPoints(LSHShadow->get9Hashes(Batch, 0).data(), 9);
		}
		LSHShadow->BuildTableAndIndex();
		ArenaBack.swap(ArenaFront);
		LSH.swap(LSHShadow);
	}

	void HungryBroadPhase::UnlockModifications()
	{
		//less sure than I was.
	}

	/////////////////////////////////////////////////////////////////////////////////////////////////
	// Currently Uninteresting Functions below this note
	//
	// I suspect we'll end up making add and remove operational using the per-thread spsc queue model
	// that we use elsewhere in barrage. At the moment, I'm going to try to defer that, because I'd _really_ like to use the BGI
	// bulk construct for the RTree and LSH update algorithms aren't well considered for cLSH. this means we gain no benefit from incremental updates
	// but that was already broadly true due to the shadow copy strategy. I think there's _actually_ a way past that, using fast mmcpy "off update"
	//
	// now, we could queue everything, mash it down every time we do the stuff, but I've checked and update finalize has
	// the body manager locked in all cases (assuming we add some functionality in lock\unlock
	// and we only need a NARROW window to generate our shadow copies.
	////////////////////////////////////////////////////////////////////////////////////////////////
	///

#define BORING_FUNCTION_REGION_OF_HBPH true
#ifdef BORING_FUNCTION_REGION_OF_HBPH
#pragma region BORON_ZONE


	void HungryBroadPhase::Init(JPH::BodyManager* inBodyManager, const JPH::BroadPhaseLayerInterface& inLayerInterface)
	{
		BroadPhase::Init(inBodyManager, inLayerInterface);
	}


	HungryBroadPhase::~HungryBroadPhase()
	{
	}

	void HungryBroadPhase::Optimize()
	{
		// this maybe should generate an out-of-band update for the shadowtree
	}

	void HungryBroadPhase::FrameSync()
	{
		BroadPhase::FrameSync();
	}

	void HungryBroadPhase::LockModifications()
	{
		//BroadPhase::LockModifications();
		//this is a no-op for us, oddly.
	}

	BroadPhase::UpdateState HungryBroadPhase::UpdatePrepare()
	{
		return BroadPhase::UpdatePrepare();
	}


	HungryBroadPhase::AddState HungryBroadPhase::AddBodiesPrepare(JPH::BodyID* ioBodies, int inNumber)
	{
		return BroadPhase::AddBodiesPrepare(ioBodies, inNumber);
	}

	void HungryBroadPhase::AddBodiesFinalize(BodyID* ioBodies, int inNumber, HungryBroadPhase::AddState inAddState)
	{
		// no op. we only remove at the tick boundary and we do it by updating from the body manager during lock.
	}

	void HungryBroadPhase::AddBodiesAbort(JPH::BodyID* ioBodies, int inNumber, AddState inAddState)
	{
		//no-op. until finalize happens, nothing happens with this model.
	}

	void HungryBroadPhase::RemoveBodies(BodyID* ioBodies, int inNumber)
	{
		// no op. we only remove at the tick boundary and we do it by updating from the body manager during lock.
	}

	void HungryBroadPhase::NotifyBodiesAABBChanged(BodyID* ioBodies, int inNumber, bool inTakeLock)
	{
		// Do nothing, we directly reference the body
	}

	void HungryBroadPhase::NotifyBodiesLayerChanged(BodyID* ioBodies, int inNumber)
	{
		// Do nothing, we directly reference the body
	}

	void HungryBroadPhase::CastRay(const RayCast& inRay, RayCastBodyCollector& ioCollector,
	                               const BroadPhaseLayerFilter& inBroadPhaseLayerFilter,
	                               const ObjectLayerFilter& inObjectLayerFilter) const
	{
		CastRay(inRay, ioCollector, inBroadPhaseLayerFilter, inObjectLayerFilter, DefaultMaxAllowedHitsPerCast);
	}

	void HungryBroadPhase::CastAABox(const AABoxCast& inBox, CastShapeBodyCollector& ioCollector,
	                                 const BroadPhaseLayerFilter& inBroadPhaseLayerFilter,
	                                 const ObjectLayerFilter& inObjectLayerFilter) const
	{
		CastAABoxNoLock(inBox, ioCollector, inBroadPhaseLayerFilter, inObjectLayerFilter);
	}
#endif
#pragma endregion
#undef BORING_FUNCTION_REGION_OF_HBPH
JPH_NAMESPACE_END
