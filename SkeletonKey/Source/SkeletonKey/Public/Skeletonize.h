//While I call it a key prefix in a few places, it's actually a infix for a variety of reasons, mostly because when
//hashfunctions do bias, they tend to MSB-bias their entropy OR fail under modulo.
//__________________________________________| 0b0000000000000000000000000000000000000000000000000000000000000000;
//__________________________________________| 0b1111111111111111111111111111111111111111111111111111111111111111;
//__________________________________________| 0bHHHHHHHHGGGGGGGGFFFFFFFFEEEEEEEE DDDDDDDDCCCCCCCCBBBBBBBBAAAAAAAA;
//__________________________________________| 0bHHHHHHHHGG1GGGGGFF1FFFFFEE1EEEEE DDD1DDDDC1CCCCCCBBB1BBBBA1AAAAAA;
//__________________________________________| 0b00000000001000000010000000100000 00010000010000000001000001000000;
//__________________________________________| 0b11111111110111111101111111011111 11101111101111111110111110111111;
#pragma once
#ifndef LORDLY_SKELETON_H
#define LORDLY_SKELETON_H
#define LORDLY_SKELETON_VER 0

namespace SKELLY
{
	constexpr static uint64_t	SFIX_MASK_OUT = 0b1111111111011111110111111101111111101111101111111110111110111111;
	constexpr static uint64_t	SFIX_MASK_EXT = 0b0000000000100000001000000010000000010000010000000001000001000000;
	constexpr static uint64_t	SFIX_NONE	  = 0b0000000000000000000000000000000000000000000000000000000000000000;
	constexpr static uint64_t	SFIX_ACT_COMP =	SFIX_MASK_EXT; //special case, since this is the basal capability.

	//An archetype or shared instance gets one of these...
	constexpr static uint64_t	SFIX_ART_GUNS = 0b0000000000000000000000000000000000000000000000000000000001000000;
	//An individual _instance_ gets one of these...
	constexpr static uint64_t	SFIX_ART_1GUN = 0b0000000000000000000000000000000000000000000000000001000001000000;
	constexpr static uint64_t	SFIX_BAR_BODS = 0b0000000000000000000000000000000000000000000000000001000000000000;
	constexpr static uint64_t	SFIX_BAR_PRIM = 0b0000000000000000000000000000000000000000010000000000000000000000;
	constexpr static uint64_t	SFIX_ART_ACTS = 0b0000000000000000000000000000000000010000000000000000000000000000;
	constexpr static uint64_t	SFIX_ART_FCMS = 0b0000000000000000000000000010000000000000000000000000000000000000;
	//I'm not going to do it for all of them, but this might help visualize things for people.
	//weirdly, this has already caught a bug or two.
	//______________________________________ _| 0b__________________1_____________________________________________;
	constexpr static uint64_t	SFIX_ART_FACT = 0b0000000000000000001000000000000000000000000000000000000000000000;
	constexpr static uint64_t	SFIX_PLAYERID = 0b0000000000100000000000000000000000000000000000000000000000000000;
	//HIGHER GAMEPLAY CONSTRUCTS, THIS TEXT IS FOR YOU ALONE.
	constexpr static uint64_t	SFIX_BONEKEY =  0b0000000000100000000000000000000000000000000000000000000001000000;
	//This is the key for constellations and constellation like meta structures.
	constexpr static uint64_t	SFIX_STELLAR =  0b0000000000100000001000000000000000000000000000000000000001000000;
}
	static inline bool IS_OF_SK_TYPE(uint64_t MY_HASH,uint64_t MY_MASK) {return (MY_HASH & SKELLY::SFIX_MASK_EXT) == MY_MASK;};
	static inline uint64_t GET_SK_TYPE(uint64_t MY_HASH) {return (MY_HASH & SKELLY::SFIX_MASK_EXT);};
	static inline uint64_t FORGE_SKELETON_KEY(uint64_t MY_HASH,uint64_t MY_MASK) {return (MY_HASH & SKELLY::SFIX_MASK_OUT) | MY_MASK;};
	constexpr uint64_t BoneKey_Infix = SKELLY::SFIX_BONEKEY;
	constexpr uint64_t GunInstance_Infix = SKELLY::SFIX_ART_1GUN;
#endif

#define MAKE_BONEKEY(turn_into_key) FBoneKey(PointerHash(turn_into_key))
#define MAKE_ACTORKEY(turn_into_key) ActorKey(PointerHash(turn_into_key))
