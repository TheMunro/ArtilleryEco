#pragma once
#include "IsolatedJoltIncludes.h"

PRAGMA_PUSH_PLATFORM_DEFAULT_PACKING
#include "Embedder.h"
#include "LSH/sketch/bbmh.h"

JPH_NAMESPACE_BEGIN
//we use a novel LSH solution with a shadow-copying approach to create
// an exceptionally fast, if memory HUNGRY, attack on the broadphase problem.
// seriously, this thing will devour your ram.


	class HungryBroadPhase : public JPH::BroadPhase
	{
		static inline constexpr uint32_t DefaultMaxAllowedHitsPerCast = 2048;
		static inline constexpr float AllowedAddedScanRangeMult = 3;
		static inline constexpr float PointsPerBB = 9;
		static inline constexpr float SpeculativeContactDistance = 0.1;
#define ___LOCO_RCFF(x) (x)
//min
#define __LOCO_FFABV(m)		 ___LOCO_RCFF(m.x), ___LOCO_RCFF(m.y), ___LOCO_RCFF(m.z) 
//max
#define __LOCO_FFABVM(m)	 ___LOCO_RCFF( (m.x + m.xBound) ), ___LOCO_RCFF((m.y + m.yBound)), ___LOCO_RCFF(m.z + m.zBound)
//center
#define __LOCO_FFABVC(m)	 ___LOCO_RCFF( (m.x + (m.xBound/2)) ), ___LOCO_RCFF((m.y + (m.yBound/2))), ___LOCO_RCFF((m.z + (m.zBound/2))) 
#define __LOCO_FFABVX(m)	 ___LOCO_RCFF( (m.x + m.xBound) ), ___LOCO_RCFF((m.y)), ___LOCO_RCFF(m.z) 
#define __LOCO_FFABVXY(m)	 ___LOCO_RCFF( (m.x + m.xBound) ), ___LOCO_RCFF((m.y + m.yBound)), ___LOCO_RCFF(m.z) 
#define __LOCO_FFABVXZ(m)	 ___LOCO_RCFF( (m.x + m.xBound) ), ___LOCO_RCFF((m.y)), ___LOCO_RCFF(m.z + m.zBound) 
#define __LOCO_FFABVY(m)	 ___LOCO_RCFF( (m.x) ), ___LOCO_RCFF((m.y + m.yBound)), ___LOCO_RCFF(m.z) 
#define __LOCO_FFABVYZ(m)	 ___LOCO_RCFF( (m.x) ), ___LOCO_RCFF((m.y + m.yBound)), ___LOCO_RCFF((m.z + m.zBound) )
#define __LOCO_FFABVZ(m)	 ___LOCO_RCFF( (m.x ) ), ___LOCO_RCFF((m.y )), ___LOCO_RCFF(m.z + m.zBound) 
	////////////////////////////////////////////////
	///BodyBoxSafeShadow
	////////////////////////////////////////////////
	//This struct is an intentional copy of certain attributes of a body sufficient to fully describe it for most steps of
	//the broadphase as well as provide a little meta info if we need it. We regenerate these copies during build, so we don't need to be
	//aware of notify in either direction. This allows us to avoid any locking and push our byte width down below a cache line fill

	//we should move layer out, as seen below, but it actually buys us nothing in most packing schemas. You could either use the below schema
	//or you could do something a bit diff where you always allocate "blobs" of these fellas such that they're pack-aligned.

	/* example 64b aligned would look something like this for our purposes.
	* 
	* struct A_BS
	{
	uint32_t meta = 0;
	//these would likely need to be unrolled into their component variables to actually avoid getting wrecked by packing padding.
	//And yes, I would waste memory before I used pack(0), because pack(0) has a lot of other costs. Profile pack0 struct access sometime.
	BodyBoxSafeShadow A;
	BodyBoxSafeShadow B;
	BodyBoxSafeShadow C;
	};
	*/
public:
		struct BodyBoxSafeShadow
		{
			///////////////////
			///The Path to 16b or less
			///////////////////
			//technically, you can save 2 bits per float from the exponent, since we don't want fractions of a centimeter
			//and 2 bits from the mantissa and still get basically the same precision, since we're going to use a minimum stride
			//That'll let you make this godforsaken thing really small using something like
			/*	    
			 *		0xZZ ZZ ZZ ZY YY YY YY XX XX XX XA BC
			 *		unsigned char xBound;
			 *		unsigned char yBound;
			 *		unsigned char zBound;
			 *		
			 *		where A, B, and C are used as exponents for xB, yB, and zB like this:
			 *		xBound * (1 << (A+1)). You can raise 1 if you need to, or structure things more like A*A*A or something.
			 *		Whatever you do, this allows you to get size classes that feel pretty okay for your stride if you fiddle.
			 *		
			 *		Layer is then defined by the _array_ the shadow is stored in.
			 *		This results in a fairly high precision, fairly accurate 16b bounding box shadow.
			 *		
			 *		However, that's pretty fiddly, unintuitive, and yields some pretty weird limits on how big objects can be.
			 *		I went ahead and just used the jolt half floats. This yields a BB of about 20b. If that turns out to be
			 *		too big or Jolt's packing takes that up to 32, I guess I'll implement the above. However, if you need to
			 *		improve cache perf, 16b is the magic number for an extra bb per 64b cache line fill,
			 *		so the above strat might be really powerful.
			 *
			 *		Here's hoping we never need it. --JMK
			 */
#define ___LOCO_CHAOSDUNK Vec3Arg{x  +  xBound, y  +  yBound, z + zBound }
			float x; //switching this to center would improve our ability to express large volumes
			float y;
			float z;
			HalfFloat xBound;
			HalfFloat yBound;
			HalfFloat zBound;
			unsigned char layer;
			BodyID meta;

			Vec3 GetCenter() const
			{
				return 0.5f * (___LOCO_CHAOSDUNK + Vec3Arg{x, y, z});
			}

			BodyBoxSafeShadow()
				: x(0),
				  y(0),
				  z(0),
				  xBound(0),
				  yBound(0),
				  zBound(0),
				  layer(0),
				  meta(0)
			{
			}

			/// Get size of bounding box
			Vec3 GetSize() const
			{
				return ___LOCO_CHAOSDUNK - Vec3Arg{x, y, z};
			}

			AABox GetWorldSpaceBounds()
			{
				//in general, compute & temp alloc is pretty cheap compared to possible cache misses. however, I'd like to ditch this.
				return AABox(Vec3Arg{x, y, z},
				             ___LOCO_CHAOSDUNK
				);
			}

			inline ObjectLayer GetObjectLayer()
			{
				return layer;
			}

			//makes a "point"
			inline BodyBoxSafeShadow(float x, float y, float z): x(x), y(y), z(z), xBound(1), yBound(1), zBound(1),
			                                                     layer(0), meta(0)
			{
			}

			inline BodyBoxSafeShadow(float x, float y, float z, HalfFloat xExt, HalfFloat yExt, HalfFloat zExt,
			                         unsigned char LayerUpTo255, JPH::BodyID ID): x(x), y(y), z(z), xBound(xExt), yBound(yExt),
			                                                      zBound(zExt), layer(LayerUpTo255), meta(ID)
			{
			}
		};

		
		JPH_OVERRIDE_NEW_DELETE

	
	double MaxExtent = 0;
		void TestInvokeBuild();
		using EMBED = Embedder<2>;
		using FLESH = EMBED::FLESH;
		using NodeBlob = std::vector<BodyBoxSafeShadow>;
		/// Handle used during adding bodies to the broadphase
		using AddState = void*;
		virtual void CastRay(const JPH::RayCast& inRay, JPH::RayCastBodyCollector& ioCollector,
		                     const JPH::BroadPhaseLayerFilter& inBroadPhaseLayerFilter = {},
		                     const JPH::ObjectLayerFilter& inObjectLayerFilter = {}) const override;
		void CastRay(const RayCast& inRay, RayCastBodyCollector& ioCollector,
		             const BroadPhaseLayerFilter& inBroadPhaseLayerFilter, const ObjectLayerFilter& inObjectLayerFilter,
		             uint32_t AllowedHits) const;
		bool ScoopUpHitsRay(RayCastBodyCollector& ioCollector, const ObjectLayerFilter& inObjectLayerFilter,
		                 uint32_t AllowedHits,
		                 Vec3 origin, RayInvDirection inv_direction, std::vector<uint32_t> tset,
		                 float early_out_fraction,
		                 uint32_t hits) const;
		void ApproxCastRay(const RayCast& inRay, RayCastBodyCollector& ioCollector,
		                   const BroadPhaseLayerFilter& inBroadPhaseLayerFilter,
		                   const ObjectLayerFilter& inObjectLayerFilter,
		                   uint32_t AllowedHits) const;
		virtual void CollideAABox(const JPH::AABox& inBox, JPH::CollideShapeBodyCollector& ioCollector,
		                          const JPH::BroadPhaseLayerFilter& inBroadPhaseLayerFilter = {},
		                          const JPH::ObjectLayerFilter& inObjectLayerFilter = {}) const override;
		virtual void CollideSphere(JPH::Vec3Arg inCenter, float inRadius, JPH::CollideShapeBodyCollector& ioCollector,
		                           const JPH::BroadPhaseLayerFilter& inBroadPhaseLayerFilter = {},
		                           const JPH::ObjectLayerFilter& inObjectLayerFilter = {}) const override;
		virtual void CollidePoint(JPH::Vec3Arg inPoint, JPH::CollideShapeBodyCollector& ioCollector,
		                          const JPH::BroadPhaseLayerFilter& inBroadPhaseLayerFilter = {},
		                          const JPH::ObjectLayerFilter& inObjectLayerFilter = {}) const override;
		virtual void CollideOrientedBox(const JPH::OrientedBox& inBox, JPH::CollideShapeBodyCollector& ioCollector,
		                                const JPH::BroadPhaseLayerFilter& inBroadPhaseLayerFilter = {},
		                                const JPH::ObjectLayerFilter& inObjectLayerFilter = {}) const override;
		template <class behavior, class T>
		bool Accumulate(const T& inBox, behavior collector, const ObjectLayerFilter& inObjectLayerFilter, float lenny,
		                UnorderedSet<uint32_t>& Setti, std::vector<uint32_t>& result) const;
		virtual void CastAABox(const JPH::AABoxCast& inBox, JPH::CastShapeBodyCollector& ioCollector,
		                       const JPH::BroadPhaseLayerFilter& inBroadPhaseLayerFilter = {},
		                       const JPH::ObjectLayerFilter& inObjectLayerFilter = {}) const override;
		virtual void Init(JPH::BodyManager* inBodyManager,
		                  const JPH::BroadPhaseLayerInterface& inLayerInterface) override;

		//legitimately, most functions do nothing with the hungry broadphase.
		virtual void Optimize() override;
		virtual void FrameSync() override;
		virtual void LockModifications() override;
		virtual UpdateState UpdatePrepare() override;
		virtual void UpdateFinalize(const UpdateState& inUpdateState) override;
		void RebuildLSH();
		virtual void UnlockModifications() override;
		virtual AddState AddBodiesPrepare(JPH::BodyID* ioBodies, int inNumber) override;
		virtual void AddBodiesFinalize(JPH::BodyID* ioBodies, int inNumber, AddState inAddState) override;

		//abort is odd. we don't support it, but we also have nothing to abort.
		virtual void AddBodiesAbort(JPH::BodyID* ioBodies, int inNumber, AddState inAddState) override;

		virtual void RemoveBodies(JPH::BodyID* ioBodies, int inNumber) override;
		virtual void NotifyBodiesAABBChanged(JPH::BodyID* ioBodies, int inNumber, bool inTakeLock = true) override;
		virtual void NotifyBodiesLayerChanged(JPH::BodyID* ioBodies, int inNumber) override;
		virtual void FindCollidingPairs(JPH::BodyID* ioActiveBodies, int inNumActiveBodies,
		                                float inSpeculativeContactDistance,
		                                const JPH::ObjectVsBroadPhaseLayerFilter& inObjectVsBroadPhaseLayerFilter,
		                                const JPH::ObjectLayerPairFilter& inObjectLayerPairFilter,
		                                JPH::BodyPairCollector& ioPairCollector) const override;
		
		virtual void CastAABoxNoLock(const JPH::AABoxCast& inBox, JPH::CastShapeBodyCollector& ioCollector,
		                             const JPH::BroadPhaseLayerFilter& inBroadPhaseLayerFilter,
		                             const JPH::ObjectLayerFilter& inObjectLayerFilter) const override;
		virtual JPH::AABox GetBounds() const override;
		void GenerateBBHashes(float oldM, std::vector<FLESHPoint>& Batch,
		                      std::vector<HungryBroadPhase::BodyBoxSafeShadow>::value_type& body);
		virtual ~HungryBroadPhase() override;

	private:
		//may need to macro this out for full platform support...
		//oh c++, never change*
		//no, seriously, please don't break ABI compatibility.
		//*as of C++20. even 11 is barely a usable language.
		FLESH::Arena ArenaFront = nullptr;
		FLESH::Arena ArenaBack = nullptr;
		std::shared_ptr<FLESH> LSH = std::make_shared<FLESH>((1024),ArenaFront);
		std::shared_ptr<FLESH> LSHShadow = std::make_shared<FLESH>((1024),ArenaBack);
		//we'll need to come back and roll these to a proper blob alloc.
		//and probably swap to LockFreeHashMap or allocation optimized hashmap. but for now, let's just get this thing WORKING
		std::shared_ptr<NodeBlob> mBodies = std::make_shared<NodeBlob>(NodeBlob(1024));
		std::shared_ptr<NodeBlob> mBodiesShadow = std::make_shared<NodeBlob>(NodeBlob(1024));
	};
PRAGMA_POP_PLATFORM_DEFAULT_PACKING
JPH_NAMESPACE_END
