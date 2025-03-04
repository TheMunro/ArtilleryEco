// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "NiagaraUIRenderer/Public/NiagaraUIComponent.h"
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodeNiagaraUIComponent() {}

// Begin Cross Module References
NIAGARA_API UClass* Z_Construct_UClass_UNiagaraComponent();
NIAGARAUIRENDERER_API UClass* Z_Construct_UClass_UNiagaraUIComponent();
NIAGARAUIRENDERER_API UClass* Z_Construct_UClass_UNiagaraUIComponent_NoRegister();
UPackage* Z_Construct_UPackage__Script_NiagaraUIRenderer();
// End Cross Module References

// Begin Class UNiagaraUIComponent
void UNiagaraUIComponent::StaticRegisterNativesUNiagaraUIComponent()
{
}
IMPLEMENT_CLASS_NO_AUTO_REGISTRATION(UNiagaraUIComponent);
UClass* Z_Construct_UClass_UNiagaraUIComponent_NoRegister()
{
	return UNiagaraUIComponent::StaticClass();
}
struct Z_Construct_UClass_UNiagaraUIComponent_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Class_MetaDataParams[] = {
#if !UE_BUILD_SHIPPING
		{ "Comment", "/**\n * \n */" },
#endif
		{ "HideCategories", "Object Physics Collision Mobility VirtualTexture" },
		{ "IncludePath", "NiagaraUIComponent.h" },
		{ "ModuleRelativePath", "Public/NiagaraUIComponent.h" },
	};
#endif // WITH_METADATA
	static UObject* (*const DependentSingletons[])();
	static constexpr FCppClassTypeInfoStatic StaticCppClassTypeInfo = {
		TCppClassTypeTraits<UNiagaraUIComponent>::IsAbstract,
	};
	static const UECodeGen_Private::FClassParams ClassParams;
};
UObject* (*const Z_Construct_UClass_UNiagaraUIComponent_Statics::DependentSingletons[])() = {
	(UObject* (*)())Z_Construct_UClass_UNiagaraComponent,
	(UObject* (*)())Z_Construct_UPackage__Script_NiagaraUIRenderer,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UClass_UNiagaraUIComponent_Statics::DependentSingletons) < 16);
const UECodeGen_Private::FClassParams Z_Construct_UClass_UNiagaraUIComponent_Statics::ClassParams = {
	&UNiagaraUIComponent::StaticClass,
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
	0x00B010A4u,
	METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UClass_UNiagaraUIComponent_Statics::Class_MetaDataParams), Z_Construct_UClass_UNiagaraUIComponent_Statics::Class_MetaDataParams)
};
UClass* Z_Construct_UClass_UNiagaraUIComponent()
{
	if (!Z_Registration_Info_UClass_UNiagaraUIComponent.OuterSingleton)
	{
		UECodeGen_Private::ConstructUClass(Z_Registration_Info_UClass_UNiagaraUIComponent.OuterSingleton, Z_Construct_UClass_UNiagaraUIComponent_Statics::ClassParams);
	}
	return Z_Registration_Info_UClass_UNiagaraUIComponent.OuterSingleton;
}
template<> NIAGARAUIRENDERER_API UClass* StaticClass<UNiagaraUIComponent>()
{
	return UNiagaraUIComponent::StaticClass();
}
UNiagaraUIComponent::UNiagaraUIComponent(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) {}
DEFINE_VTABLE_PTR_HELPER_CTOR(UNiagaraUIComponent);
UNiagaraUIComponent::~UNiagaraUIComponent() {}
// End Class UNiagaraUIComponent

// Begin Registration
struct Z_CompiledInDeferFile_FID_Users_jakek_Documents_Unreal_Projects_Bristle54_5_5_plugins_NiagaraUIRenderer_Source_NiagaraUIRenderer_Public_NiagaraUIComponent_h_Statics
{
	static constexpr FClassRegisterCompiledInInfo ClassInfo[] = {
		{ Z_Construct_UClass_UNiagaraUIComponent, UNiagaraUIComponent::StaticClass, TEXT("UNiagaraUIComponent"), &Z_Registration_Info_UClass_UNiagaraUIComponent, CONSTRUCT_RELOAD_VERSION_INFO(FClassReloadVersionInfo, sizeof(UNiagaraUIComponent), 3512652040U) },
	};
};
static FRegisterCompiledInInfo Z_CompiledInDeferFile_FID_Users_jakek_Documents_Unreal_Projects_Bristle54_5_5_plugins_NiagaraUIRenderer_Source_NiagaraUIRenderer_Public_NiagaraUIComponent_h_505568604(TEXT("/Script/NiagaraUIRenderer"),
	Z_CompiledInDeferFile_FID_Users_jakek_Documents_Unreal_Projects_Bristle54_5_5_plugins_NiagaraUIRenderer_Source_NiagaraUIRenderer_Public_NiagaraUIComponent_h_Statics::ClassInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_Users_jakek_Documents_Unreal_Projects_Bristle54_5_5_plugins_NiagaraUIRenderer_Source_NiagaraUIRenderer_Public_NiagaraUIComponent_h_Statics::ClassInfo),
	nullptr, 0,
	nullptr, 0);
// End Registration
PRAGMA_ENABLE_DEPRECATION_WARNINGS
