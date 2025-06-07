//Fast Line-Embedding Sketch Hasher implementation
//MIT License
//FLINNG originally copyright (c) 2021 Joshua Engels
//Sketch is by dnbaker.
//FLESH copyright 2025, Oversized Sun
//https://www.youtube.com/watch?v=vUJEG3tUVaY
#pragma once
#include "LocomoUtil.h"

THIRD_PARTY_INCLUDES_START
PRAGMA_PUSH_PLATFORM_DEFAULT_PACKING
//THE FLESH DESIRES AN ARENA.
//THE ARENA DESIRES AN ALLOCATOR.
//okay technically, TLSF isn't an arena allocator, but we actually use it much like one.
//The Embedder's FLESH data structure is basically a very large inverted lookup based on FLINNG.
//It does a HUGE sweep of allocations but never modifies itself. So we want to be able to blow it up
//in a single call. That pretty much means a linear, pool, or arena allocator. Unfortunately,
//we don't have a great idea of what size one of the vector components is. So we allocate a large pool
//then use TLSF to manage it, basically creating arena regions representing each of the FLESH structures.
#include "IsolatedJoltIncludes.h"
//as portable as a brick through your windshield. It'll drive.
#include "Sorts/IntelSimdSorts/x86simdsort-static-incl.h"
#include "ZOrderDistances.h"
#include "LSH/sketch/bbmh.h"
#include <memory>
typedef FZOrderDistances::FPLEmbed FLESHPoint;
THIRD_PARTY_INCLUDES_END

//FLINNG-driven index for point/line embeddings. See the FPLEmbed implementation for links to the
//paper that inspired it. These high-d vectors have most of the properties we would associate with
//sparse data, so FLineESH defaults to something close to the sparse model laid out in the FLINNG readme.
//Sketch provides our hasher.
template <uint32_t num_rows> //see algorithm 1 from https://arxiv.org/pdf/2106.11565 (pg 4-5)//bit width per hash, basically.
          
class Embedder
{
public:
	using CDVType = uint64_t;
	using CHAOSDUNKER = sketch::BBitMinHasher<CDVType>; //cause it's time to play BBitball.
	using HASHER = CHAOSDUNKER; //fine. I'll play along. 
	using ALLOC_INNER = IntraTickThreadblindAlloc<uint32_t>;
	using INTERIORTYPE = std::vector<uint32_t, ALLOC_INNER>;
	using ALLOC_OUTER = IntraTickThreadblindAlloc<INTERIORTYPE>;

	//Normally, I'd make this a parametric as a template arg but under the hood, Sketch packs the bits of the hash
	//and doesn't expose the raw hashes. There's not really normally a reason to for their use case.
	//So you'd need a number of hash tables that is a multiple of 4. 8 hash tables is A LOT. so 4 it is.
	constexpr static uint8_t hash_range_pow	  = 16;
	constexpr static uint32_t num_hash_tables = 4;
									 //   _   _   _   _
	constexpr static uint64_t mask = 0x000000000000FFFF;
	constexpr static uint32_t hash_range = (1 << hash_range_pow);
	//initial bytes of the sketch, permutes? it looks like it's permutes? I'm not sure. apparently this can one-perm????
	
	//previously, moving this into the inner class caused some template whackiness due to default constructors.
	//we've switched away from that allocator, due to other issues around memory overhead. I still wouldn't move this in
	//as it actually does make more sense out here. It's not really part of the actual machinery.
	struct HashBlob
	{
		uint64_t Blob[num_hash_tables];
	};

	static HashBlob& getHash(FLESHPoint& data,
					 uint32_t seed)
	{
		thread_local HashBlob ret;
		alt_dense_hash(ret.Blob, data);
		return ret;
	}

	static void alt_dense_hash(CDVType* result, FLESHPoint& data)
	{
		static thread_local HASHER BBitMinHashInst = HASHER(log2( num_hash_tables),hash_range_pow);
		BBitMinHashInst.addh(data.voxeldata[0]);
		BBitMinHashInst.addh(data.voxeldata[1]);
		BBitMinHashInst.addh(data.voxeldata[2]);
		BBitMinHashInst.addh(data.voxeldata[3]);
		BBitMinHashInst.addh(data.voxeldata[4]);
		//BBitMinHashInst.densify();//do we needdddddddddd this?!
		//these two steps together MAY blank out the data? we may not need to do it, either? I can't tell yet.
		auto res = BBitMinHashInst.finalize(hash_range_pow);
		for (int BitPackedBin = 0; BitPackedBin < num_hash_tables/4; ++BitPackedBin)
		{
			
			for (int UnpackOffset = 0; UnpackOffset < 4; ++UnpackOffset) // unpackku
			{
				result[BitPackedBin+UnpackOffset] = (res.core_[BitPackedBin]
													 >> (hash_range_pow*UnpackOffset))
													 & mask;
			}
		}
		BBitMinHashInst.reset();
		//now what???
	}
	
	class FLESH
	{
	public:
		
		using Arena = IntraTickThreadblindAlloc<uint32_t>::TrueLifecycleManager;
	private:

		using RawPool = IntraTickThreadblindAlloc<uint32_t>::POOL*;
		using Cells = std::vector<std::vector<uint32_t, ALLOC_INNER>,ALLOC_OUTER>;
		using InvertInd = std::vector<std::vector<uint32_t, ALLOC_INNER>, ALLOC_OUTER>;
		ALLOC_INNER ints_alloc = ALLOC_INNER();//INTS ALLOC IS A BLIND CHILD.
		ALLOC_OUTER vecs_alloc = ALLOC_OUTER();//THIS ISN'T A METAPHOR. it uses vecs alloc's pool.
		uint64_t cells_per_row;
		uint64_t total_points_added = 0;
		bool IHaveAPool = false;
		static constexpr size_t PoolSize = 64*1024*1024;
		RawPool ForMemsetClear = nullptr; 
		//so I tried to make this an std::array and it causes some templating fun. ultimately, I went with vector+reserve.
		//this is for legibility reasons and because I think I'll need to structurally rework the entire design ove the invert index, tbh.
		Cells* cell_membership;
		InvertInd* inverted_flinng_index;
		

	
	public:
		Arena INTERNAL_ARENA;
		FLESH(): cells_per_row(0), cell_membership(nullptr), inverted_flinng_index(nullptr)
		{
		}

		//the act of unknowing is never safe.
		//it is often necessary. remember, the buddha is a wooden duck.
		~FLESH()
		{
			if (IHaveAPool && INTERNAL_ARENA) // if I think I have a pool and I still hold a strong ref to the arena
			{
				memset(ForMemsetClear, 0,   PoolSize);
			}
			
		}

		// This is the main entry point for querying.
		std::vector<uint32_t> query(HashBlob& hash, uint32_t top_k)
		{
			std::vector<uint32_t> results(top_k);

			std::vector<uint32_t> counts(num_rows * cells_per_row, 0);
			if (ProcessQueryBlob(hash, top_k, results, 0, counts))
			{
				return results; // early exit due to k met.
			}
			return results;
		}
		
		
		//when flesh is destroyed, it memsets the pool but DOES NOT DESTROY IT unless you discard that out param.
		//it will do the right thing either way, functionally.
		FLESH(uint64_t DataSize, Arena& OUT_PARAM_ARENALIFECYCLE) 
		: cells_per_row(std::max(DataSize, 1ull)), //c'mon. man.
		vecs_alloc(PoolSize)
		{
			INTERNAL_ARENA = vecs_alloc.Init(); //DO NOT COMBINE WITH BELOW. the currying works in an odd way due to inlining or SOMETHING.
			//look, if you so badly want to save zero operations, because the compiler ALMOST CERTAINLY optimizes this, it'd actually make me really happy.
			//oh, thought I'd be snarky? no. This is like a bur I can't polish out. We just don't have time right now, and it's honestly a little clearer this way
			//but it doesn't quite match the usual allocate-at-use style we prefer. in fact, none of this is very RAII.
			ints_alloc = ALLOC_INNER(vecs_alloc.UnderlyingTlsf, ints_alloc.InitialPool, vecs_alloc.Size);
			//it's not worth using less than 4 uint32 elements in a vector. unfortunately, I think this may make it allocate 8.
			//Then you get into issues with cache line fills. This is genuinely not good.
			//Heyyyyy sooooooo..... Don't ever do this. We actually _clear the memory pool itself_ so we don't want to call dealloc on these guys.
			cell_membership = new Cells(num_rows * DataSize,INTERIORTYPE(8,0, ints_alloc), vecs_alloc);
			inverted_flinng_index = new InvertInd(hash_range * num_hash_tables, INTERIORTYPE(32,0,ints_alloc), vecs_alloc);
			ForMemsetClear = vecs_alloc.InitialPool;
			OUT_PARAM_ARENALIFECYCLE = INTERNAL_ARENA;
			IHaveAPool = true;
		}

		//consign what you were to the void.
		void RegenerateTheFLESH(uint64_t DataSize)
		{
			vecs_alloc.DoNotUseThis();
			cells_per_row = std::max(DataSize, 1ull);
			ints_alloc = ALLOC_INNER(vecs_alloc.UnderlyingTlsf, ints_alloc.InitialPool, vecs_alloc.Size);
			//it's not worth using less than 4 uint32 elements in a vector. unfortunately, I think this may make it allocate 8.
			//Then you get into issues with cache line fills. This is genuinely not good.
			cell_membership = new Cells(num_rows * DataSize,INTERIORTYPE(4,0, ints_alloc), vecs_alloc);
			inverted_flinng_index = new InvertInd(hash_range * num_hash_tables, INTERIORTYPE(32,0,ints_alloc), vecs_alloc);
		}
		
		std::vector<HashBlob> getHashes(std::vector<FLESHPoint> data,
										uint32_t seed)
		{
			return densified_minhash(data, seed);
		}

		std::array<HashBlob, 9>& get9Hashes(std::vector<FLESHPoint>& data,
									uint32_t seed)
		{
			return densified_minhash<9>(data, seed);
		}



		//this just abstracts away the particulars of hashing a little. :/
		//if you need to switch back to dense mode, it'll be nice to have.
		inline std::vector<HashBlob>
		densified_minhash(std::vector<FLESHPoint>& points, uint32_t random_seed)
		{
			std::vector<HashBlob> result(points.size());
			for (uint64_t point_id = 0; point_id < points.size(); point_id += 1)
			{
				alt_dense_hash(result[point_id].Blob,
										 points[point_id]);
			}
			return result;
		}

		template<int FixedPointCount>
		inline std::array<HashBlob,FixedPointCount>&
		densified_minhash(std::vector<FLESHPoint>& points, uint32_t random_seed)
		{
			thread_local std::array<HashBlob,FixedPointCount> result = {};
			for (uint64_t point_id = 0; point_id < points.size(); point_id += 1)
			{
				alt_dense_hash(result[point_id].Blob,
										 points[point_id]);
			}
			return result;
		}



		////TABLE funcs

		// All the hashes for point 1 come first, etc.
		// Size of hashes should be multiple of num_hash_tables
		void addHashedPoints(HashBlob* hashes, uint64_t num_points)
		{
			std::vector<uint32_t> random_buckets(num_rows * num_points);

			//do we need to regenerate this each time? I think we do not...
			for (uint32_t i = 0; i < num_rows * num_points; i++)
			{
				//reserves 0 for empty.
				random_buckets[i] =
					std::max( 1ull,(rand() % cells_per_row + cells_per_row) % cells_per_row +
					(i % num_rows) * cells_per_row);
			}

			for (uint64_t table = 0; table < num_hash_tables; table++)
			{
				for (uint64_t point = 0; point < num_points; point++)
				{
					uint64_t hash = hashes[point].Blob[table];
					uint64_t hash_id = table * hash_range + hash;
					for (uint64_t row = 0; row < num_rows; row++)
					{
						(*inverted_flinng_index)[hash_id].push_back(
							random_buckets[point * num_rows + row]);
					}
				}
			}

			for (uint64_t point = 0; point < num_points; point++)
			{
				for (uint64_t row = 0; row < num_rows; row++)
				{
					const auto idx = point * num_rows + row;
					if (idx < (*cell_membership).size())
					{
						(*cell_membership)[idx].push_back(
						total_points_added + point);
					}
					else
					{
						throw; //temp
					}
				}
			}

			total_points_added += num_points;
		}

		// this is the version that should be the fastest
		static inline const __m128i pass1_add4 = _mm_setr_epi32(1, 1, 3, 3);
		static inline const __m128i pass2_add4 = _mm_setr_epi32(2, 3, 2, 3);
		//and a wooden duck is duck enough for most people, most of the time.
		static inline const __m128i pass3_add4 = _mm_setr_epi32(0, 2, 2, 3);
		void BuildTableAndIndex()
		{
			//currently, this assumes at least four members of a cell, which we ensure with reserve, but this CAN lead to zeros sneaking in
			//later, we ignore those manually. [IGNOREDET] for that code.
			for (uint64_t i = 0; i < (*inverted_flinng_index).size(); i++)
			{
				
				_m_prefetch((*inverted_flinng_index).data());

				auto v = (*inverted_flinng_index)[i];
				x86simdsortStatic::qsort(v.data(), v.size());
				const auto [first, last] = std::ranges::unique(v.begin(), v.end());
				v.erase(first, last);

			}
		}

		bool ProcessQueryBlob(HashBlob& hash, uint32_t top_k, std::vector<uint32_t> results, uint32_t query_id,
							  std::vector<uint32_t> counts)
		{
			for (uint32_t rep = 0; rep < num_hash_tables; rep++)
			{
				const uint32_t index = hash_range * rep + hash.Blob[rep];
				std::vector<uint32_t, ALLOC_INNER>& alloc_inner = (*inverted_flinng_index)[index];
				const uint32_t size = alloc_inner.size();
				for (uint32_t small_index = 0; small_index < size; small_index++)
				{
					// This single line takes 80% of the time, around half for the move
					// and half for the add
					counts[alloc_inner[small_index]] += alloc_inner[small_index] > 0;
				}
			}

			std::vector<uint32_t> sorted[num_hash_tables + 1];//welllll, the count bins are sorted already.........? is that... enough?
			uint32_t size_guess = num_rows * cells_per_row / (num_hash_tables + 1);
			for (std::vector<uint32_t>& v : sorted)
			{
				v.reserve(size_guess);
			}

			for (uint32_t i = 0; i < num_rows * cells_per_row; ++i)
			{
				sorted[counts[i]].push_back(i);
			}

			if (num_rows > 2)
			{
				std::vector<uint8_t> num_counts(total_points_added, 0);
				uint32_t num_found = 0;
				for (int32_t rep = num_hash_tables; rep >= 0; --rep)
				{
					for (uint32_t bin : sorted[rep])
					{
						for (uint32_t point : (*cell_membership)[bin])
						{
							if (++num_counts[point] == num_rows)
							{
								results.push_back(point);
								if (++num_found == top_k)
								{
									return true;
								}
							}
						}
					}
				}
			}
			else
			{
				char* num_counts =
					(char*)calloc(total_points_added / 8 + 1, sizeof(char));
				uint32_t num_found = 0;
				for (int32_t rep = num_hash_tables; rep >= 0; --rep)
				{
					for (uint32_t bin : sorted[rep])
					{
						for (uint32_t point : (*cell_membership)[bin])
						{
							if (num_counts[(point / 8)] & (1 << (point % 8))) //what?
							{
								results.push_back(point);
								if (++num_found == top_k)
								{
									free(num_counts);
									return true; //I swear it was like this when I found it.
								}
							}
							else
							{
								num_counts[(point / 8)] |= (1 << (point % 8));
							}
						}
					}
				}
			}
			return false;
		}

		// Again all the hashes for point 1 come first, etc.
		// Size of hashes should be multiple of num_hash_tables
		// Results are similarly ordered
		std::vector<uint32_t> query(HashBlob* hashes, size_t queries, uint32_t top_k)
		{
			std::vector<uint32_t> results(top_k * queries);
			Qui(hashes, top_k, queries, results);
			return results;
		}

		//must be hashes.len*top_k long.
		std::vector<uint32_t> query_byoa(HashBlob* hashes, size_t queries, uint32_t top_k, std::vector<uint32_t>& results)
		{
			Qui(hashes, top_k, queries, results);
			
			return results;
		}

		void Qui(HashBlob* hashes, uint32_t top_k, uint64_t num_queries, std::vector<uint32_t>& results)
		{
			for (uint32_t query_id = 0; query_id < num_queries; query_id++)
			{
				std::vector<uint32_t> counts(num_rows * cells_per_row, 0);
				ProcessQueryBlob(hashes[query_id], top_k, results, query_id, counts);
			}
		}

	};
};
PRAGMA_POP_PLATFORM_DEFAULT_PACKING		