// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "NiagaraUIRenderer/Public/NiagaraUIActor.h"
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodeNiagaraUIActor() {}

// Begin Cross Module References
ENGINE_API UClass* Z_Construct_UClass_AActor();
NIAGARAUIRENDERER_API UClass* Z_Construct_UClass_ANiagaraUIActor();
NIAGARAUIRENDERER_API UClass* Z_Construct_UClass_ANiagaraUIActor_NoRegister();
UPackage* Z_Construct_UPackage__Script_NiagaraUIRenderer();
// End Cross Module References

// Begin Class ANiagaraUIActor
void ANiagaraUIActor::StaticRegisterNativesANiagaraUIActor()
{
}
IMPLEMENT_CLASS_NO_AUTO_REGISTRATION(ANiagaraUIActor);
UClass* Z_Construct_UClass_ANiagaraUIActor_NoRegister()
{
	return ANiagaraUIActor::StaticClass();
}
struct Z_Construct_UClass_ANiagaraUIActor_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Class_MetaDataParams[] = {
		{ "IncludePath", "NiagaraUIActor.h" },
		{ "ModuleRelativePath", "Public/NiagaraUIActor.h" },
	};
#endif // WITH_METADATA
	static UObject* (*const DependentSingletons[])();
	static constexpr FCppClassTypeInfoStatic StaticCppClassTypeInfo = {
		TCppClassTypeTraits<ANiagaraUIActor>::IsAbstract,
	};
	static const UECodeGen_Private::FClassParams ClassParams;
};
UObject* (*const Z_Construct_UClass_ANiagaraUIActor_Statics::DependentSingletons[])() = {
	(UObject* (*)())Z_Construct_UClass_AActor,
	(UObject* (*)())Z_Construct_UPackage__Script_NiagaraUIRenderer,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UClass_ANiagaraUIActor_Statics::DependentSingletons) < 16);
const UECodeGen_Private::FClassParams Z_Construct_UClass_ANiagaraUIActor_Statics::ClassParams = {
	&ANiagaraUIActor::StaticClass,
	"Engine",
	&StaticCppClassTypeInfo,
	DependentSingletons,
	nullptr,
	nullptr,
	nullptr,
	UE_ARRAY_COUNT(DependentSingletons),
	0,
	0,
	0,
	0x008000A4u,
	METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UClass_ANiagaraUIActor_Statics::Class_MetaDataParams), Z_Construct_UClass_ANiagaraUIActor_Statics::Class_MetaDataParams)
};
UClass* Z_Construct_UClass_ANiagaraUIActor()
{
	if (!Z_Registration_Info_UClass_ANiagaraUIActor.OuterSingleton)
	{
		UECodeGen_Private::ConstructUClass(Z_Registration_Info_UClass_ANiagaraUIActor.OuterSingleton, Z_Construct_UClass_ANiagaraUIActor_Statics::ClassParams);
	}
	return Z_Registration_Info_UClass_ANiagaraUIActor.OuterSingleton;
}
template<> NIAGARAUIRENDERER_API UClass* StaticClass<ANiagaraUIActor>()
{
	return ANiagaraUIActor::StaticClass();
}
DEFINE_VTABLE_PTR_HELPER_CTOR(ANiagaraUIActor);
ANiagaraUIActor::~ANiagaraUIActor() {}
// End Class ANiagaraUIActor

// Begin Registration
struct Z_CompiledInDeferFile_FID_Users_jakek_Documents_Unreal_Projects_Bristle54_5_5_plugins_NiagaraUIRenderer_Source_NiagaraUIRenderer_Public_NiagaraUIActor_h_Statics
{
	static constexpr FClassRegisterCompiledInInfo ClassInfo[] = {
		{ Z_Construct_UClass_ANiagaraUIActor, ANiagaraUIActor::StaticClass, TEXT("ANiagaraUIActor"), &Z_Registration_Info_UClass_ANiagaraUIActor, CONSTRUCT_RELOAD_VERSION_INFO(FClassReloadVersionInfo, sizeof(ANiagaraUIActor), 1961174520U) },
	};
};
static FRegisterCompiledInInfo Z_CompiledInDeferFile_FID_Users_jakek_Documents_Unreal_Projects_Bristle54_5_5_plugins_NiagaraUIRenderer_Source_NiagaraUIRenderer_Public_NiagaraUIActor_h_3571201841(TEXT("/Script/NiagaraUIRenderer"),
	Z_CompiledInDeferFile_FID_Users_jakek_Documents_Unreal_Projects_Bristle54_5_5_plugins_NiagaraUIRenderer_Source_NiagaraUIRenderer_Public_NiagaraUIActor_h_Statics::ClassInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_Users_jakek_Documents_Unreal_Projects_Bristle54_5_5_plugins_NiagaraUIRenderer_Source_NiagaraUIRenderer_Public_NiagaraUIActor_h_Statics::ClassInfo),
	nullptr, 0,
	nullptr, 0);
// End Registration
PRAGMA_ENABLE_DEPRECATION_WARNINGS
