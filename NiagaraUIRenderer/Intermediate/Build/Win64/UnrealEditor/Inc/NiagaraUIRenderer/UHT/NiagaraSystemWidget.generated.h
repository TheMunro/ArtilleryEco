// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

// IWYU pragma: private, include "NiagaraSystemWidget.h"
#include "UObject/ObjectMacros.h"
#include "UObject/ScriptMacros.h"

PRAGMA_DISABLE_DEPRECATION_WARNINGS
class UMaterialInterface;
class UNiagaraSystem;
class UNiagaraUIComponent;
struct FLinearColor;
#ifdef NIAGARAUIRENDERER_NiagaraSystemWidget_generated_h
#error "NiagaraSystemWidget.generated.h already included, missing '#pragma once' in NiagaraSystemWidget.h"
#endif
#define NIAGARAUIRENDERER_NiagaraSystemWidget_generated_h

#define FID_Users_jakek_Documents_Unreal_Projects_Bristle54_5_5_plugins_NiagaraUIRenderer_Source_NiagaraUIRenderer_Public_NiagaraSystemWidget_h_19_RPC_WRAPPERS_NO_PURE_DECLS \
	DECLARE_FUNCTION(execSetColorAndOpacity); \
	DECLARE_FUNCTION(execGetRemapMaterial); \
	DECLARE_FUNCTION(execSetRemapMaterial); \
	DECLARE_FUNCTION(execSetDesiredWidgetSize); \
	DECLARE_FUNCTION(execUpdateTickWhenPaused); \
	DECLARE_FUNCTION(execUpdateNiagaraSystemReference); \
	DECLARE_FUNCTION(execGetNiagaraComponent); \
	DECLARE_FUNCTION(execDeactivateSystem); \
	DECLARE_FUNCTION(execActivateSystem);


#define FID_Users_jakek_Documents_Unreal_Projects_Bristle54_5_5_plugins_NiagaraUIRenderer_Source_NiagaraUIRenderer_Public_NiagaraSystemWidget_h_19_ACCESSORS \
static void GetDesiredWidgetSize_WrapperImpl(const void* Object, void* OutValue); \
static void SetDesiredWidgetSize_WrapperImpl(void* Object, const void* InValue); \
static void GetColorAndOpacity_WrapperImpl(const void* Object, void* OutValue); \
static void SetColorAndOpacity_WrapperImpl(void* Object, const void* InValue);


#define FID_Users_jakek_Documents_Unreal_Projects_Bristle54_5_5_plugins_NiagaraUIRenderer_Source_NiagaraUIRenderer_Public_NiagaraSystemWidget_h_19_INCLASS_NO_PURE_DECLS \
private: \
	static void StaticRegisterNativesUNiagaraSystemWidget(); \
	friend struct Z_Construct_UClass_UNiagaraSystemWidget_Statics; \
public: \
	DECLARE_CLASS(UNiagaraSystemWidget, UWidget, COMPILED_IN_FLAGS(0), CASTCLASS_None, TEXT("/Script/NiagaraUIRenderer"), NO_API) \
	DECLARE_SERIALIZER(UNiagaraSystemWidget)


#define FID_Users_jakek_Documents_Unreal_Projects_Bristle54_5_5_plugins_NiagaraUIRenderer_Source_NiagaraUIRenderer_Public_NiagaraSystemWidget_h_19_ENHANCED_CONSTRUCTORS \
private: \
	/** Private move- and copy-constructors, should never be used */ \
	UNiagaraSystemWidget(UNiagaraSystemWidget&&); \
	UNiagaraSystemWidget(const UNiagaraSystemWidget&); \
public: \
	DECLARE_VTABLE_PTR_HELPER_CTOR(NO_API, UNiagaraSystemWidget); \
	DEFINE_VTABLE_PTR_HELPER_CTOR_CALLER(UNiagaraSystemWidget); \
	DEFINE_DEFAULT_OBJECT_INITIALIZER_CONSTRUCTOR_CALL(UNiagaraSystemWidget) \
	NO_API virtual ~UNiagaraSystemWidget();


#define FID_Users_jakek_Documents_Unreal_Projects_Bristle54_5_5_plugins_NiagaraUIRenderer_Source_NiagaraUIRenderer_Public_NiagaraSystemWidget_h_16_PROLOG
#define FID_Users_jakek_Documents_Unreal_Projects_Bristle54_5_5_plugins_NiagaraUIRenderer_Source_NiagaraUIRenderer_Public_NiagaraSystemWidget_h_19_GENERATED_BODY \
PRAGMA_DISABLE_DEPRECATION_WARNINGS \
public: \
	FID_Users_jakek_Documents_Unreal_Projects_Bristle54_5_5_plugins_NiagaraUIRenderer_Source_NiagaraUIRenderer_Public_NiagaraSystemWidget_h_19_RPC_WRAPPERS_NO_PURE_DECLS \
	FID_Users_jakek_Documents_Unreal_Projects_Bristle54_5_5_plugins_NiagaraUIRenderer_Source_NiagaraUIRenderer_Public_NiagaraSystemWidget_h_19_ACCESSORS \
	FID_Users_jakek_Documents_Unreal_Projects_Bristle54_5_5_plugins_NiagaraUIRenderer_Source_NiagaraUIRenderer_Public_NiagaraSystemWidget_h_19_INCLASS_NO_PURE_DECLS \
	FID_Users_jakek_Documents_Unreal_Projects_Bristle54_5_5_plugins_NiagaraUIRenderer_Source_NiagaraUIRenderer_Public_NiagaraSystemWidget_h_19_ENHANCED_CONSTRUCTORS \
private: \
PRAGMA_ENABLE_DEPRECATION_WARNINGS


template<> NIAGARAUIRENDERER_API UClass* StaticClass<class UNiagaraSystemWidget>();

#undef CURRENT_FILE_ID
#define CURRENT_FILE_ID FID_Users_jakek_Documents_Unreal_Projects_Bristle54_5_5_plugins_NiagaraUIRenderer_Source_NiagaraUIRenderer_Public_NiagaraSystemWidget_h


PRAGMA_ENABLE_DEPRECATION_WARNINGS
