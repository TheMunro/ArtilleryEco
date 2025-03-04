//base.h
#pragma once
#include <typeinfo>
#include <algorithm>
#include <map>

template<std::size_t N>
struct CompTimeStr {
	char data[N] {};

	consteval CompTimeStr(const char (&str)[N]) {
		std::copy_n(str, N, data);
	}

	consteval bool operator==(const CompTimeStr<N> str) const {
		return std::equal(str.data, str.data+N, data);
	}

	template<std::size_t N2>
	consteval bool operator==(const CompTimeStr<N2> s) const {
		return false;
	}

	template<std::size_t N2>
	consteval CompTimeStr<N+N2-1> operator+(const CompTimeStr<N2> str) const {
		char newchar[N+N2-1] {};
		std::copy_n(data, N-1, newchar);
		std::copy_n(str.data, N2, newchar+N-1);
		return newchar;
	}

	consteval char operator[](std::size_t n) const {
		return data[n];
	}

	consteval std::size_t size() const {
		return N-1;
	}
};

template<std::size_t s1, std::size_t s2>
consteval auto operator+(CompTimeStr<s1> fs, const char (&str) [s2]) {
	return fs + CompTimeStr<s2>(str);
}

template<std::size_t s1, std::size_t s2>
consteval auto operator+(const char (&str) [s2], CompTimeStr<s1> fs) {
	return CompTimeStr<s2>(str) + fs;
}

template<std::size_t s1, std::size_t s2>
consteval auto operator==(CompTimeStr<s1> fs, const char (&str) [s2]) {
	return fs == CompTimeStr<s2>(str);
}

template<std::size_t s1, std::size_t s2>
consteval auto operator==(const char (&str) [s2], CompTimeStr<s1> fs) {
	return CompTimeStr<s2>(str) == fs;
}

//https://stackoverflow.com/questions/49374705/can-i-get-the-current-class-type-id-from-a-static-base-method
//this is a form of CRTP
//I wasn't able to find a nice way to skip requiring every artillery gun to implement an inner class extending the
//base class yet, so this is out of service for the time being. If you want to wire it up, it's a cool project.
template<typename... Args>
auto sum(Args... args)
{
	return (args + ...);
}

template<class MasterType>
struct BaseStructDecorator
{
public:
	static inline TMap<FString, MasterType*> Registrar;
};


//The following is a working example that ultimately didn't manage to avoid getting scrobbled by the weird memory tricks UEEd uses for PIE.
//We ended up using data tables instead, and I recommend you do the same, sadly.
/*
*#define AGBindGunId(GUN) namespace GunAggr{ const static RegisterMyGun<GUN, #GUN> Identification_##GUN; const bool  _bind##GUN  = RegisterMyGun<GUN, #GUN>::ReentrySafeRegister();}

static constexpr auto GunAggregator = BaseStructDecorator<FArtilleryGun>();
extern inline auto GunAggregatorBind = &GunAggregator;

template<typename Gun, CompTimeStr Name>
struct RegisterMyGun : public BaseStructDecorator<FArtilleryGun>
{

public:
RegisterMyGun()
{
IInst = ReentrySafeRegister();
}

friend Gun;
static inline TSharedPtr<FArtilleryGun> LateBind;
static inline FArtilleryGun* Bind;
static bool CheckDone(bool set = false)
{
static bool MyWorkIsDone = false;
if(set)
{
MyWorkIsDone = true;
}
return MyWorkIsDone;
}
[[nodiscard]] static bool ReentrySafeRegister()
{
if(CheckDone() || Bind == nullptr)
{
CheckDone(true);//don't move this. just trust me.
if(LateBind == nullptr)
{
Bind = reinterpret_cast<FArtilleryGun*>(new Gun());
LateBind = MakeShareable(Bind); //this manages our lifecycle, and thanks to the checkdone, means we only allocate once.
//if we get sandblasted, we will allocate again next call.
}
FString MyName = FString(Name.data);
UE_LOG(LogTemp, Log, TEXT("RegisterMyGun: Gun definition loaded as %s"), *MyName);
GunAggregatorBind->Registrar.insert_or_assign(MyName, Bind);
UE_LOG(LogTemp, Log, TEXT("RegisterMyGun: Gun definitions right now: %llu"), GunAggregatorBind->Registrar.size());
}
return CheckDone();
};
bool IInst;
	
};

*/