// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "NiagaraUIRenderer/Public/NiagaraSystemWidget.h"
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodeNiagaraSystemWidget() {}

// Begin Cross Module References
COREUOBJECT_API UScriptStruct* Z_Construct_UScriptStruct_FLinearColor();
COREUOBJECT_API UScriptStruct* Z_Construct_UScriptStruct_FVector2D();
ENGINE_API UClass* Z_Construct_UClass_UMaterialInterface_NoRegister();
NIAGARA_API UClass* Z_Construct_UClass_UNiagaraSystem_NoRegister();
NIAGARAUIRENDERER_API UClass* Z_Construct_UClass_ANiagaraUIActor_NoRegister();
NIAGARAUIRENDERER_API UClass* Z_Construct_UClass_UNiagaraSystemWidget();
NIAGARAUIRENDERER_API UClass* Z_Construct_UClass_UNiagaraSystemWidget_NoRegister();
NIAGARAUIRENDERER_API UClass* Z_Construct_UClass_UNiagaraUIComponent_NoRegister();
UMG_API UClass* Z_Construct_UClass_UWidget();
UPackage* Z_Construct_UPackage__Script_NiagaraUIRenderer();
// End Cross Module References

// Begin Class UNiagaraSystemWidget Function ActivateSystem
struct Z_Construct_UFunction_UNiagaraSystemWidget_ActivateSystem_Statics
{
	struct NiagaraSystemWidget_eventActivateSystem_Parms
	{
		bool Reset;
	};
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "Category", "Niagara UI Renderer" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "// Activate Niagara System with option to reset the simulation\n" },
#endif
		{ "ModuleRelativePath", "Public/NiagaraSystemWidget.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "Activate Niagara System with option to reset the simulation" },
#endif
	};
#endif // WITH_METADATA
	static void NewProp_Reset_SetBit(void* Obj);
	static const UECodeGen_Private::FBoolPropertyParams NewProp_Reset;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
	static const UECodeGen_Private::FFunctionParams FuncParams;
};
void Z_Construct_UFunction_UNiagaraSystemWidget_ActivateSystem_Statics::NewProp_Reset_SetBit(void* Obj)
{
	((NiagaraSystemWidget_eventActivateSystem_Parms*)Obj)->Reset = 1;
}
const UECodeGen_Private::FBoolPropertyParams Z_Construct_UFunction_UNiagaraSystemWidget_ActivateSystem_Statics::NewProp_Reset = { "Reset", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Bool | UECodeGen_Private::EPropertyGenFlags::NativeBool, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, sizeof(bool), sizeof(NiagaraSystemWidget_eventActivateSystem_Parms), &Z_Construct_UFunction_UNiagaraSystemWidget_ActivateSystem_Statics::NewProp_Reset_SetBit, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UNiagaraSystemWidget_ActivateSystem_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UNiagaraSystemWidget_ActivateSystem_Statics::NewProp_Reset,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_UNiagaraSystemWidget_ActivateSystem_Statics::PropPointers) < 2048);
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UNiagaraSystemWidget_ActivateSystem_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_UNiagaraSystemWidget, nullptr, "ActivateSystem", nullptr, nullptr, Z_Construct_UFunction_UNiagaraSystemWidget_ActivateSystem_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_UNiagaraSystemWidget_ActivateSystem_Statics::PropPointers), sizeof(Z_Construct_UFunction_UNiagaraSystemWidget_ActivateSystem_Statics::NiagaraSystemWidget_eventActivateSystem_Parms), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x04020401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UNiagaraSystemWidget_ActivateSystem_Statics::Function_MetaDataParams), Z_Construct_UFunction_UNiagaraSystemWidget_ActivateSystem_Statics::Function_MetaDataParams) };
static_assert(sizeof(Z_Construct_UFunction_UNiagaraSystemWidget_ActivateSystem_Statics::NiagaraSystemWidget_eventActivateSystem_Parms) < MAX_uint16);
UFunction* Z_Construct_UFunction_UNiagaraSystemWidget_ActivateSystem()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UNiagaraSystemWidget_ActivateSystem_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(UNiagaraSystemWidget::execActivateSystem)
{
	P_GET_UBOOL(Z_Param_Reset);
	P_FINISH;
	P_NATIVE_BEGIN;
	P_THIS->ActivateSystem(Z_Param_Reset);
	P_NATIVE_END;
}
// End Class UNiagaraSystemWidget Function ActivateSystem

// Begin Class UNiagaraSystemWidget Function DeactivateSystem
struct Z_Construct_UFunction_UNiagaraSystemWidget_DeactivateSystem_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "Category", "Niagara UI Renderer" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "// Deactivate Niagara System\n" },
#endif
		{ "ModuleRelativePath", "Public/NiagaraSystemWidget.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "Deactivate Niagara System" },
#endif
	};
#endif // WITH_METADATA
	static const UECodeGen_Private::FFunctionParams FuncParams;
};
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UNiagaraSystemWidget_DeactivateSystem_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_UNiagaraSystemWidget, nullptr, "DeactivateSystem", nullptr, nullptr, nullptr, 0, 0, RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x04020401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UNiagaraSystemWidget_DeactivateSystem_Statics::Function_MetaDataParams), Z_Construct_UFunction_UNiagaraSystemWidget_DeactivateSystem_Statics::Function_MetaDataParams) };
UFunction* Z_Construct_UFunction_UNiagaraSystemWidget_DeactivateSystem()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UNiagaraSystemWidget_DeactivateSystem_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(UNiagaraSystemWidget::execDeactivateSystem)
{
	P_FINISH;
	P_NATIVE_BEGIN;
	P_THIS->DeactivateSystem();
	P_NATIVE_END;
}
// End Class UNiagaraSystemWidget Function DeactivateSystem

// Begin Class UNiagaraSystemWidget Function GetNiagaraComponent
struct Z_Construct_UFunction_UNiagaraSystemWidget_GetNiagaraComponent_Statics
{
	struct NiagaraSystemWidget_eventGetNiagaraComponent_Parms
	{
		UNiagaraUIComponent* ReturnValue;
	};
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "Category", "Niagara UI Renderer" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "// Return Niagara Component reference for the particle system.\n" },
#endif
		{ "ModuleRelativePath", "Public/NiagaraSystemWidget.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "Return Niagara Component reference for the particle system." },
#endif
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_ReturnValue_MetaData[] = {
		{ "EditInline", "true" },
	};
#endif // WITH_METADATA
	static const UECodeGen_Private::FObjectPropertyParams NewProp_ReturnValue;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
	static const UECodeGen_Private::FFunctionParams FuncParams;
};
const UECodeGen_Private::FObjectPropertyParams Z_Construct_UFunction_UNiagaraSystemWidget_GetNiagaraComponent_Statics::NewProp_ReturnValue = { "ReturnValue", nullptr, (EPropertyFlags)0x0010000000080588, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(NiagaraSystemWidget_eventGetNiagaraComponent_Parms, ReturnValue), Z_Construct_UClass_UNiagaraUIComponent_NoRegister, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_ReturnValue_MetaData), NewProp_ReturnValue_MetaData) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UNiagaraSystemWidget_GetNiagaraComponent_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UNiagaraSystemWidget_GetNiagaraComponent_Statics::NewProp_ReturnValue,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_UNiagaraSystemWidget_GetNiagaraComponent_Statics::PropPointers) < 2048);
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UNiagaraSystemWidget_GetNiagaraComponent_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_UNiagaraSystemWidget, nullptr, "GetNiagaraComponent", nullptr, nullptr, Z_Construct_UFunction_UNiagaraSystemWidget_GetNiagaraComponent_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_UNiagaraSystemWidget_GetNiagaraComponent_Statics::PropPointers), sizeof(Z_Construct_UFunction_UNiagaraSystemWidget_GetNiagaraComponent_Statics::NiagaraSystemWidget_eventGetNiagaraComponent_Parms), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x14020401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UNiagaraSystemWidget_GetNiagaraComponent_Statics::Function_MetaDataParams), Z_Construct_UFunction_UNiagaraSystemWidget_GetNiagaraComponent_Statics::Function_MetaDataParams) };
static_assert(sizeof(Z_Construct_UFunction_UNiagaraSystemWidget_GetNiagaraComponent_Statics::NiagaraSystemWidget_eventGetNiagaraComponent_Parms) < MAX_uint16);
UFunction* Z_Construct_UFunction_UNiagaraSystemWidget_GetNiagaraComponent()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UNiagaraSystemWidget_GetNiagaraComponent_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(UNiagaraSystemWidget::execGetNiagaraComponent)
{
	P_FINISH;
	P_NATIVE_BEGIN;
	*(UNiagaraUIComponent**)Z_Param__Result=P_THIS->GetNiagaraComponent();
	P_NATIVE_END;
}
// End Class UNiagaraSystemWidget Function GetNiagaraComponent

// Begin Class UNiagaraSystemWidget Function GetRemapMaterial
struct Z_Construct_UFunction_UNiagaraSystemWidget_GetRemapMaterial_Statics
{
	struct NiagaraSystemWidget_eventGetRemapMaterial_Parms
	{
		UMaterialInterface* OriginalMaterial;
		UMaterialInterface* ReturnValue;
	};
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "Category", "Niagara UI Renderer" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "// Returns a remap material for a given original particle material specified in the Niagara Emitter \n" },
#endif
		{ "ModuleRelativePath", "Public/NiagaraSystemWidget.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "Returns a remap material for a given original particle material specified in the Niagara Emitter" },
#endif
	};
#endif // WITH_METADATA
	static const UECodeGen_Private::FObjectPropertyParams NewProp_OriginalMaterial;
	static const UECodeGen_Private::FObjectPropertyParams NewProp_ReturnValue;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
	static const UECodeGen_Private::FFunctionParams FuncParams;
};
const UECodeGen_Private::FObjectPropertyParams Z_Construct_UFunction_UNiagaraSystemWidget_GetRemapMaterial_Statics::NewProp_OriginalMaterial = { "OriginalMaterial", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(NiagaraSystemWidget_eventGetRemapMaterial_Parms, OriginalMaterial), Z_Construct_UClass_UMaterialInterface_NoRegister, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FObjectPropertyParams Z_Construct_UFunction_UNiagaraSystemWidget_GetRemapMaterial_Statics::NewProp_ReturnValue = { "ReturnValue", nullptr, (EPropertyFlags)0x0010000000000580, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(NiagaraSystemWidget_eventGetRemapMaterial_Parms, ReturnValue), Z_Construct_UClass_UMaterialInterface_NoRegister, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UNiagaraSystemWidget_GetRemapMaterial_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UNiagaraSystemWidget_GetRemapMaterial_Statics::NewProp_OriginalMaterial,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UNiagaraSystemWidget_GetRemapMaterial_Statics::NewProp_ReturnValue,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_UNiagaraSystemWidget_GetRemapMaterial_Statics::PropPointers) < 2048);
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UNiagaraSystemWidget_GetRemapMaterial_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_UNiagaraSystemWidget, nullptr, "GetRemapMaterial", nullptr, nullptr, Z_Construct_UFunction_UNiagaraSystemWidget_GetRemapMaterial_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_UNiagaraSystemWidget_GetRemapMaterial_Statics::PropPointers), sizeof(Z_Construct_UFunction_UNiagaraSystemWidget_GetRemapMaterial_Statics::NiagaraSystemWidget_eventGetRemapMaterial_Parms), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x04020401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UNiagaraSystemWidget_GetRemapMaterial_Statics::Function_MetaDataParams), Z_Construct_UFunction_UNiagaraSystemWidget_GetRemapMaterial_Statics::Function_MetaDataParams) };
static_assert(sizeof(Z_Construct_UFunction_UNiagaraSystemWidget_GetRemapMaterial_Statics::NiagaraSystemWidget_eventGetRemapMaterial_Parms) < MAX_uint16);
UFunction* Z_Construct_UFunction_UNiagaraSystemWidget_GetRemapMaterial()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UNiagaraSystemWidget_GetRemapMaterial_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(UNiagaraSystemWidget::execGetRemapMaterial)
{
	P_GET_OBJECT(UMaterialInterface,Z_Param_OriginalMaterial);
	P_FINISH;
	P_NATIVE_BEGIN;
	*(UMaterialInterface**)Z_Param__Result=P_THIS->GetRemapMaterial(Z_Param_OriginalMaterial);
	P_NATIVE_END;
}
// End Class UNiagaraSystemWidget Function GetRemapMaterial

// Begin Class UNiagaraSystemWidget Function SetColorAndOpacity
struct Z_Construct_UFunction_UNiagaraSystemWidget_SetColorAndOpacity_Statics
{
	struct NiagaraSystemWidget_eventSetColorAndOpacity_Parms
	{
		FLinearColor InColorAndOpacity;
	};
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "Category", "Niagara UI Renderer" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "// Set the color and opacity to a specified value\n" },
#endif
		{ "ModuleRelativePath", "Public/NiagaraSystemWidget.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "Set the color and opacity to a specified value" },
#endif
	};
#endif // WITH_METADATA
	static const UECodeGen_Private::FStructPropertyParams NewProp_InColorAndOpacity;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
	static const UECodeGen_Private::FFunctionParams FuncParams;
};
const UECodeGen_Private::FStructPropertyParams Z_Construct_UFunction_UNiagaraSystemWidget_SetColorAndOpacity_Statics::NewProp_InColorAndOpacity = { "InColorAndOpacity", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(NiagaraSystemWidget_eventSetColorAndOpacity_Parms, InColorAndOpacity), Z_Construct_UScriptStruct_FLinearColor, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UNiagaraSystemWidget_SetColorAndOpacity_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UNiagaraSystemWidget_SetColorAndOpacity_Statics::NewProp_InColorAndOpacity,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_UNiagaraSystemWidget_SetColorAndOpacity_Statics::PropPointers) < 2048);
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UNiagaraSystemWidget_SetColorAndOpacity_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_UNiagaraSystemWidget, nullptr, "SetColorAndOpacity", nullptr, nullptr, Z_Construct_UFunction_UNiagaraSystemWidget_SetColorAndOpacity_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_UNiagaraSystemWidget_SetColorAndOpacity_Statics::PropPointers), sizeof(Z_Construct_UFunction_UNiagaraSystemWidget_SetColorAndOpacity_Statics::NiagaraSystemWidget_eventSetColorAndOpacity_Parms), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x04820401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UNiagaraSystemWidget_SetColorAndOpacity_Statics::Function_MetaDataParams), Z_Construct_UFunction_UNiagaraSystemWidget_SetColorAndOpacity_Statics::Function_MetaDataParams) };
static_assert(sizeof(Z_Construct_UFunction_UNiagaraSystemWidget_SetColorAndOpacity_Statics::NiagaraSystemWidget_eventSetColorAndOpacity_Parms) < MAX_uint16);
UFunction* Z_Construct_UFunction_UNiagaraSystemWidget_SetColorAndOpacity()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UNiagaraSystemWidget_SetColorAndOpacity_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(UNiagaraSystemWidget::execSetColorAndOpacity)
{
	P_GET_STRUCT(FLinearColor,Z_Param_InColorAndOpacity);
	P_FINISH;
	P_NATIVE_BEGIN;
	P_THIS->SetColorAndOpacity(Z_Param_InColorAndOpacity);
	P_NATIVE_END;
}
// End Class UNiagaraSystemWidget Function SetColorAndOpacity

// Begin Class UNiagaraSystemWidget Function SetDesiredWidgetSize
struct Z_Construct_UFunction_UNiagaraSystemWidget_SetDesiredWidgetSize_Statics
{
	struct NiagaraSystemWidget_eventSetDesiredWidgetSize_Parms
	{
		FVector2D NewDesiredSize;
	};
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "Category", "Niagara UI Renderer" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "// Updates the desired widget size. If calling from code call this instead of setting the DesiredWidgetSize directly.\n" },
#endif
		{ "ModuleRelativePath", "Public/NiagaraSystemWidget.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "Updates the desired widget size. If calling from code call this instead of setting the DesiredWidgetSize directly." },
#endif
	};
#endif // WITH_METADATA
	static const UECodeGen_Private::FStructPropertyParams NewProp_NewDesiredSize;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
	static const UECodeGen_Private::FFunctionParams FuncParams;
};
const UECodeGen_Private::FStructPropertyParams Z_Construct_UFunction_UNiagaraSystemWidget_SetDesiredWidgetSize_Statics::NewProp_NewDesiredSize = { "NewDesiredSize", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(NiagaraSystemWidget_eventSetDesiredWidgetSize_Parms, NewDesiredSize), Z_Construct_UScriptStruct_FVector2D, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UNiagaraSystemWidget_SetDesiredWidgetSize_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UNiagaraSystemWidget_SetDesiredWidgetSize_Statics::NewProp_NewDesiredSize,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_UNiagaraSystemWidget_SetDesiredWidgetSize_Statics::PropPointers) < 2048);
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UNiagaraSystemWidget_SetDesiredWidgetSize_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_UNiagaraSystemWidget, nullptr, "SetDesiredWidgetSize", nullptr, nullptr, Z_Construct_UFunction_UNiagaraSystemWidget_SetDesiredWidgetSize_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_UNiagaraSystemWidget_SetDesiredWidgetSize_Statics::PropPointers), sizeof(Z_Construct_UFunction_UNiagaraSystemWidget_SetDesiredWidgetSize_Statics::NiagaraSystemWidget_eventSetDesiredWidgetSize_Parms), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x04820401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UNiagaraSystemWidget_SetDesiredWidgetSize_Statics::Function_MetaDataParams), Z_Construct_UFunction_UNiagaraSystemWidget_SetDesiredWidgetSize_Statics::Function_MetaDataParams) };
static_assert(sizeof(Z_Construct_UFunction_UNiagaraSystemWidget_SetDesiredWidgetSize_Statics::NiagaraSystemWidget_eventSetDesiredWidgetSize_Parms) < MAX_uint16);
UFunction* Z_Construct_UFunction_UNiagaraSystemWidget_SetDesiredWidgetSize()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UNiagaraSystemWidget_SetDesiredWidgetSize_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(UNiagaraSystemWidget::execSetDesiredWidgetSize)
{
	P_GET_STRUCT(FVector2D,Z_Param_NewDesiredSize);
	P_FINISH;
	P_NATIVE_BEGIN;
	P_THIS->SetDesiredWidgetSize(Z_Param_NewDesiredSize);
	P_NATIVE_END;
}
// End Class UNiagaraSystemWidget Function SetDesiredWidgetSize

// Begin Class UNiagaraSystemWidget Function SetRemapMaterial
struct Z_Construct_UFunction_UNiagaraSystemWidget_SetRemapMaterial_Statics
{
	struct NiagaraSystemWidget_eventSetRemapMaterial_Parms
	{
		UMaterialInterface* OriginalMaterial;
		UMaterialInterface* RemapMaterial;
	};
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "Category", "Niagara UI Renderer" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "/** \n\x09 *\x09Sets / Updates a remap material for a source particle material\n\x09 *\x09@param\x09OriginalMaterial\x09Source material specified in the Niagara Emitter that should be remapped to a new one\n\x09 *\x09@param\x09RemapMaterial\x09\x09""A new remap material that should be used when rendering UI\n\x09 */" },
#endif
		{ "ModuleRelativePath", "Public/NiagaraSystemWidget.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "Sets / Updates a remap material for a source particle material\n@param  OriginalMaterial        Source material specified in the Niagara Emitter that should be remapped to a new one\n@param  RemapMaterial           A new remap material that should be used when rendering UI" },
#endif
	};
#endif // WITH_METADATA
	static const UECodeGen_Private::FObjectPropertyParams NewProp_OriginalMaterial;
	static const UECodeGen_Private::FObjectPropertyParams NewProp_RemapMaterial;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
	static const UECodeGen_Private::FFunctionParams FuncParams;
};
const UECodeGen_Private::FObjectPropertyParams Z_Construct_UFunction_UNiagaraSystemWidget_SetRemapMaterial_Statics::NewProp_OriginalMaterial = { "OriginalMaterial", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(NiagaraSystemWidget_eventSetRemapMaterial_Parms, OriginalMaterial), Z_Construct_UClass_UMaterialInterface_NoRegister, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FObjectPropertyParams Z_Construct_UFunction_UNiagaraSystemWidget_SetRemapMaterial_Statics::NewProp_RemapMaterial = { "RemapMaterial", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(NiagaraSystemWidget_eventSetRemapMaterial_Parms, RemapMaterial), Z_Construct_UClass_UMaterialInterface_NoRegister, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UNiagaraSystemWidget_SetRemapMaterial_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UNiagaraSystemWidget_SetRemapMaterial_Statics::NewProp_OriginalMaterial,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UNiagaraSystemWidget_SetRemapMaterial_Statics::NewProp_RemapMaterial,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_UNiagaraSystemWidget_SetRemapMaterial_Statics::PropPointers) < 2048);
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UNiagaraSystemWidget_SetRemapMaterial_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_UNiagaraSystemWidget, nullptr, "SetRemapMaterial", nullptr, nullptr, Z_Construct_UFunction_UNiagaraSystemWidget_SetRemapMaterial_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_UNiagaraSystemWidget_SetRemapMaterial_Statics::PropPointers), sizeof(Z_Construct_UFunction_UNiagaraSystemWidget_SetRemapMaterial_Statics::NiagaraSystemWidget_eventSetRemapMaterial_Parms), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x04020401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UNiagaraSystemWidget_SetRemapMaterial_Statics::Function_MetaDataParams), Z_Construct_UFunction_UNiagaraSystemWidget_SetRemapMaterial_Statics::Function_MetaDataParams) };
static_assert(sizeof(Z_Construct_UFunction_UNiagaraSystemWidget_SetRemapMaterial_Statics::NiagaraSystemWidget_eventSetRemapMaterial_Parms) < MAX_uint16);
UFunction* Z_Construct_UFunction_UNiagaraSystemWidget_SetRemapMaterial()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UNiagaraSystemWidget_SetRemapMaterial_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(UNiagaraSystemWidget::execSetRemapMaterial)
{
	P_GET_OBJECT(UMaterialInterface,Z_Param_OriginalMaterial);
	P_GET_OBJECT(UMaterialInterface,Z_Param_RemapMaterial);
	P_FINISH;
	P_NATIVE_BEGIN;
	P_THIS->SetRemapMaterial(Z_Param_OriginalMaterial,Z_Param_RemapMaterial);
	P_NATIVE_END;
}
// End Class UNiagaraSystemWidget Function SetRemapMaterial

// Begin Class UNiagaraSystemWidget Function UpdateNiagaraSystemReference
struct Z_Construct_UFunction_UNiagaraSystemWidget_UpdateNiagaraSystemReference_Statics
{
	struct NiagaraSystemWidget_eventUpdateNiagaraSystemReference_Parms
	{
		UNiagaraSystem* NewNiagaraSystem;
	};
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "Category", "Niagara UI Renderer" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "// Updates the Niagara System reference. This will cause the particle system to reset\n" },
#endif
		{ "ModuleRelativePath", "Public/NiagaraSystemWidget.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "Updates the Niagara System reference. This will cause the particle system to reset" },
#endif
	};
#endif // WITH_METADATA
	static const UECodeGen_Private::FObjectPropertyParams NewProp_NewNiagaraSystem;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
	static const UECodeGen_Private::FFunctionParams FuncParams;
};
const UECodeGen_Private::FObjectPropertyParams Z_Construct_UFunction_UNiagaraSystemWidget_UpdateNiagaraSystemReference_Statics::NewProp_NewNiagaraSystem = { "NewNiagaraSystem", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(NiagaraSystemWidget_eventUpdateNiagaraSystemReference_Parms, NewNiagaraSystem), Z_Construct_UClass_UNiagaraSystem_NoRegister, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UNiagaraSystemWidget_UpdateNiagaraSystemReference_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UNiagaraSystemWidget_UpdateNiagaraSystemReference_Statics::NewProp_NewNiagaraSystem,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_UNiagaraSystemWidget_UpdateNiagaraSystemReference_Statics::PropPointers) < 2048);
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UNiagaraSystemWidget_UpdateNiagaraSystemReference_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_UNiagaraSystemWidget, nullptr, "UpdateNiagaraSystemReference", nullptr, nullptr, Z_Construct_UFunction_UNiagaraSystemWidget_UpdateNiagaraSystemReference_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_UNiagaraSystemWidget_UpdateNiagaraSystemReference_Statics::PropPointers), sizeof(Z_Construct_UFunction_UNiagaraSystemWidget_UpdateNiagaraSystemReference_Statics::NiagaraSystemWidget_eventUpdateNiagaraSystemReference_Parms), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x04020401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UNiagaraSystemWidget_UpdateNiagaraSystemReference_Statics::Function_MetaDataParams), Z_Construct_UFunction_UNiagaraSystemWidget_UpdateNiagaraSystemReference_Statics::Function_MetaDataParams) };
static_assert(sizeof(Z_Construct_UFunction_UNiagaraSystemWidget_UpdateNiagaraSystemReference_Statics::NiagaraSystemWidget_eventUpdateNiagaraSystemReference_Parms) < MAX_uint16);
UFunction* Z_Construct_UFunction_UNiagaraSystemWidget_UpdateNiagaraSystemReference()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UNiagaraSystemWidget_UpdateNiagaraSystemReference_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(UNiagaraSystemWidget::execUpdateNiagaraSystemReference)
{
	P_GET_OBJECT(UNiagaraSystem,Z_Param_NewNiagaraSystem);
	P_FINISH;
	P_NATIVE_BEGIN;
	P_THIS->UpdateNiagaraSystemReference(Z_Param_NewNiagaraSystem);
	P_NATIVE_END;
}
// End Class UNiagaraSystemWidget Function UpdateNiagaraSystemReference

// Begin Class UNiagaraSystemWidget Function UpdateTickWhenPaused
struct Z_Construct_UFunction_UNiagaraSystemWidget_UpdateTickWhenPaused_Statics
{
	struct NiagaraSystemWidget_eventUpdateTickWhenPaused_Parms
	{
		bool NewTickWhenPaused;
	};
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "Category", "Niagara UI Renderer" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "// Updates Tick When Paused - Should be this particle system updated even when the game is paused? Note that this will reset the particle simulation\n" },
#endif
		{ "ModuleRelativePath", "Public/NiagaraSystemWidget.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "Updates Tick When Paused - Should be this particle system updated even when the game is paused? Note that this will reset the particle simulation" },
#endif
	};
#endif // WITH_METADATA
	static void NewProp_NewTickWhenPaused_SetBit(void* Obj);
	static const UECodeGen_Private::FBoolPropertyParams NewProp_NewTickWhenPaused;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
	static const UECodeGen_Private::FFunctionParams FuncParams;
};
void Z_Construct_UFunction_UNiagaraSystemWidget_UpdateTickWhenPaused_Statics::NewProp_NewTickWhenPaused_SetBit(void* Obj)
{
	((NiagaraSystemWidget_eventUpdateTickWhenPaused_Parms*)Obj)->NewTickWhenPaused = 1;
}
const UECodeGen_Private::FBoolPropertyParams Z_Construct_UFunction_UNiagaraSystemWidget_UpdateTickWhenPaused_Statics::NewProp_NewTickWhenPaused = { "NewTickWhenPaused", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Bool | UECodeGen_Private::EPropertyGenFlags::NativeBool, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, sizeof(bool), sizeof(NiagaraSystemWidget_eventUpdateTickWhenPaused_Parms), &Z_Construct_UFunction_UNiagaraSystemWidget_UpdateTickWhenPaused_Statics::NewProp_NewTickWhenPaused_SetBit, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UNiagaraSystemWidget_UpdateTickWhenPaused_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UNiagaraSystemWidget_UpdateTickWhenPaused_Statics::NewProp_NewTickWhenPaused,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_UNiagaraSystemWidget_UpdateTickWhenPaused_Statics::PropPointers) < 2048);
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UNiagaraSystemWidget_UpdateTickWhenPaused_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_UNiagaraSystemWidget, nullptr, "UpdateTickWhenPaused", nullptr, nullptr, Z_Construct_UFunction_UNiagaraSystemWidget_UpdateTickWhenPaused_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_UNiagaraSystemWidget_UpdateTickWhenPaused_Statics::PropPointers), sizeof(Z_Construct_UFunction_UNiagaraSystemWidget_UpdateTickWhenPaused_Statics::NiagaraSystemWidget_eventUpdateTickWhenPaused_Parms), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x04020401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UNiagaraSystemWidget_UpdateTickWhenPaused_Statics::Function_MetaDataParams), Z_Construct_UFunction_UNiagaraSystemWidget_UpdateTickWhenPaused_Statics::Function_MetaDataParams) };
static_assert(sizeof(Z_Construct_UFunction_UNiagaraSystemWidget_UpdateTickWhenPaused_Statics::NiagaraSystemWidget_eventUpdateTickWhenPaused_Parms) < MAX_uint16);
UFunction* Z_Construct_UFunction_UNiagaraSystemWidget_UpdateTickWhenPaused()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UNiagaraSystemWidget_UpdateTickWhenPaused_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(UNiagaraSystemWidget::execUpdateTickWhenPaused)
{
	P_GET_UBOOL(Z_Param_NewTickWhenPaused);
	P_FINISH;
	P_NATIVE_BEGIN;
	P_THIS->UpdateTickWhenPaused(Z_Param_NewTickWhenPaused);
	P_NATIVE_END;
}
// End Class UNiagaraSystemWidget Function UpdateTickWhenPaused

// Begin Class UNiagaraSystemWidget
void UNiagaraSystemWidget::GetDesiredWidgetSize_WrapperImpl(const void* Object, void* OutValue)
{
	const UNiagaraSystemWidget* Obj = (const UNiagaraSystemWidget*)Object;
	FVector2D& Result = *(FVector2D*)OutValue;
	Result = (FVector2D)Obj->GetDesiredWidgetSize();
}
void UNiagaraSystemWidget::SetDesiredWidgetSize_WrapperImpl(void* Object, const void* InValue)
{
	UNiagaraSystemWidget* Obj = (UNiagaraSystemWidget*)Object;
	FVector2D& Value = *(FVector2D*)InValue;
	Obj->SetDesiredWidgetSize(Value);
}
void UNiagaraSystemWidget::GetColorAndOpacity_WrapperImpl(const void* Object, void* OutValue)
{
	const UNiagaraSystemWidget* Obj = (const UNiagaraSystemWidget*)Object;
	FLinearColor& Result = *(FLinearColor*)OutValue;
	Result = (FLinearColor)Obj->GetColorAndOpacity();
}
void UNiagaraSystemWidget::SetColorAndOpacity_WrapperImpl(void* Object, const void* InValue)
{
	UNiagaraSystemWidget* Obj = (UNiagaraSystemWidget*)Object;
	FLinearColor& Value = *(FLinearColor*)InValue;
	Obj->SetColorAndOpacity(Value);
}
void UNiagaraSystemWidget::StaticRegisterNativesUNiagaraSystemWidget()
{
	UClass* Class = UNiagaraSystemWidget::StaticClass();
	static const FNameNativePtrPair Funcs[] = {
		{ "ActivateSystem", &UNiagaraSystemWidget::execActivateSystem },
		{ "DeactivateSystem", &UNiagaraSystemWidget::execDeactivateSystem },
		{ "GetNiagaraComponent", &UNiagaraSystemWidget::execGetNiagaraComponent },
		{ "GetRemapMaterial", &UNiagaraSystemWidget::execGetRemapMaterial },
		{ "SetColorAndOpacity", &UNiagaraSystemWidget::execSetColorAndOpacity },
		{ "SetDesiredWidgetSize", &UNiagaraSystemWidget::execSetDesiredWidgetSize },
		{ "SetRemapMaterial", &UNiagaraSystemWidget::execSetRemapMaterial },
		{ "UpdateNiagaraSystemReference", &UNiagaraSystemWidget::execUpdateNiagaraSystemReference },
		{ "UpdateTickWhenPaused", &UNiagaraSystemWidget::execUpdateTickWhenPaused },
	};
	FNativeFunctionRegistrar::RegisterFunctions(Class, Funcs, UE_ARRAY_COUNT(Funcs));
}
IMPLEMENT_CLASS_NO_AUTO_REGISTRATION(UNiagaraSystemWidget);
UClass* Z_Construct_UClass_UNiagaraSystemWidget_NoRegister()
{
	return UNiagaraSystemWidget::StaticClass();
}
struct Z_Construct_UClass_UNiagaraSystemWidget_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Class_MetaDataParams[] = {
#if !UE_BUILD_SHIPPING
		{ "Comment", "/**\n The Niagara System Widget allows to render niagara particle system directly into the UI. Only sprite and ribbon CPU particles are supported.\n */" },
#endif
		{ "IncludePath", "NiagaraSystemWidget.h" },
		{ "ModuleRelativePath", "Public/NiagaraSystemWidget.h" },
		{ "ObjectInitializerConstructorDeclared", "" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "The Niagara System Widget allows to render niagara particle system directly into the UI. Only sprite and ribbon CPU particles are supported." },
#endif
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_NiagaraSystemReference_MetaData[] = {
		{ "BlueprintSetter", "UpdateNiagaraSystemReference" },
		{ "Category", "Niagara UI Renderer" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "// Reference to the niagara system asset\n" },
#endif
		{ "DisplayName", "Niagara System" },
		{ "ModuleRelativePath", "Public/NiagaraSystemWidget.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "Reference to the niagara system asset" },
#endif
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_MaterialRemapList_MetaData[] = {
		{ "Category", "Niagara UI Renderer" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "/*\n\x09\x09List of material references used to remap materials on the particle system, to materials with \"User Interface\" material domain.\n\n\x09\x09""Every Key (Material on the left) will be remapped to its Value (Material on the right)\n\n\x09\x09This is useful for keeping the particle system rendering correctly in the niagara editor and in the world, while it still can be used as UI particle system.\n\n\x09\x09The alternative is to apply materials with \"User Interface\" material domain directly in niagara renderers, but this will result in particle system\n\x09\x09not rendering correctly, if used outside UI renderer.\n\x09*/" },
#endif
		{ "ModuleRelativePath", "Public/NiagaraSystemWidget.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "List of material references used to remap materials on the particle system, to materials with \"User Interface\" material domain.\n\nEvery Key (Material on the left) will be remapped to its Value (Material on the right)\n\nThis is useful for keeping the particle system rendering correctly in the niagara editor and in the world, while it still can be used as UI particle system.\n\nThe alternative is to apply materials with \"User Interface\" material domain directly in niagara renderers, but this will result in particle system\nnot rendering correctly, if used outside UI renderer." },
#endif
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_AutoActivate_MetaData[] = {
		{ "Category", "Niagara UI Renderer" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "// Should be this particle system automatically activated?\n" },
#endif
		{ "ModuleRelativePath", "Public/NiagaraSystemWidget.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "Should be this particle system automatically activated?" },
#endif
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_TickWhenPaused_MetaData[] = {
		{ "BlueprintSetter", "UpdateTickWhenPaused" },
		{ "Category", "Niagara UI Renderer" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "// Should be this particle system updated even when the game is paused?\n" },
#endif
		{ "ModuleRelativePath", "Public/NiagaraSystemWidget.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "Should be this particle system updated even when the game is paused?" },
#endif
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_DesiredWidgetSize_MetaData[] = {
		{ "BlueprintSetter", "SetDesiredWidgetSize" },
		{ "Category", "Niagara UI Renderer" },
		{ "ClampMin", "0.000000" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "// The size of this particle widget used when calculating it's desired size. Don't set directly from code, call SetDesiredWidgetSize instead.\n" },
#endif
		{ "ModuleRelativePath", "Public/NiagaraSystemWidget.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "The size of this particle widget used when calculating it's desired size. Don't set directly from code, call SetDesiredWidgetSize instead." },
#endif
		{ "UIMax", "4096.000000" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_ColorAndOpacity_MetaData[] = {
		{ "BlueprintSetter", "SetColorAndOpacity" },
		{ "Category", "Niagara UI Renderer" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "// This color will be used as a multiplier with the particle color to calculate the final vertex color\n" },
#endif
		{ "ModuleRelativePath", "Public/NiagaraSystemWidget.h" },
		{ "sRGB", "true" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "This color will be used as a multiplier with the particle color to calculate the final vertex color" },
#endif
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_FakeDepthScale_MetaData[] = {
		{ "Category", "Niagara UI Renderer" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "// Scale particles based on their position in Y-axis (towards the camera)\n" },
#endif
		{ "ModuleRelativePath", "Public/NiagaraSystemWidget.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "Scale particles based on their position in Y-axis (towards the camera)" },
#endif
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_FakeDepthScaleDistance_MetaData[] = {
		{ "Category", "Niagara UI Renderer" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "// Fake distance from camera if the particle is at 0 0 0 - Particles will be getting bigger quicker the lower this number is\n" },
#endif
		{ "EditCondition", "FakeDepthScale" },
		{ "ModuleRelativePath", "Public/NiagaraSystemWidget.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "Fake distance from camera if the particle is at 0 0 0 - Particles will be getting bigger quicker the lower this number is" },
#endif
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_ShowDebugSystemInWorld_MetaData[] = {
		{ "Category", "Niagara UI Renderer" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "// Show debug particle system we're rendering in the game world. It'll be near 0 0 0\n" },
#endif
		{ "ModuleRelativePath", "Public/NiagaraSystemWidget.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "Show debug particle system we're rendering in the game world. It'll be near 0 0 0" },
#endif
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_PassDynamicParametersFromRibbon_MetaData[] = {
		{ "Category", "Niagara UI Renderer" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "// Should the dynamic parameters R and G channels be passed as UV1 coordinates from ribbon renderers? This replaces the generated UV1 coordinates, which on the use case might be worth the tradeoff.\n" },
#endif
		{ "ModuleRelativePath", "Public/NiagaraSystemWidget.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "Should the dynamic parameters R and G channels be passed as UV1 coordinates from ribbon renderers? This replaces the generated UV1 coordinates, which on the use case might be worth the tradeoff." },
#endif
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_DisableWarnings_MetaData[] = {
		{ "Category", "Niagara UI Renderer" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "// Disable warnings for this Widget\n" },
#endif
		{ "ModuleRelativePath", "Public/NiagaraSystemWidget.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "Disable warnings for this Widget" },
#endif
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_RestartSimulationOnPropertyChange_MetaData[] = {
		{ "Category", "Niagara UI Renderer" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "// Should the system restart simulation when a property is changed\n" },
#endif
		{ "ModuleRelativePath", "Public/NiagaraSystemWidget.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "Should the system restart simulation when a property is changed" },
#endif
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_NiagaraActor_MetaData[] = {
		{ "ModuleRelativePath", "Public/NiagaraSystemWidget.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_NiagaraComponent_MetaData[] = {
		{ "EditInline", "true" },
		{ "ModuleRelativePath", "Public/NiagaraSystemWidget.h" },
	};
#endif // WITH_METADATA
	static const UECodeGen_Private::FObjectPropertyParams NewProp_NiagaraSystemReference;
	static const UECodeGen_Private::FObjectPropertyParams NewProp_MaterialRemapList_ValueProp;
	static const UECodeGen_Private::FObjectPropertyParams NewProp_MaterialRemapList_Key_KeyProp;
	static const UECodeGen_Private::FMapPropertyParams NewProp_MaterialRemapList;
	static void NewProp_AutoActivate_SetBit(void* Obj);
	static const UECodeGen_Private::FBoolPropertyParams NewProp_AutoActivate;
	static void NewProp_TickWhenPaused_SetBit(void* Obj);
	static const UECodeGen_Private::FBoolPropertyParams NewProp_TickWhenPaused;
	static const UECodeGen_Private::FStructPropertyParams NewProp_DesiredWidgetSize;
	static const UECodeGen_Private::FStructPropertyParams NewProp_ColorAndOpacity;
	static void NewProp_FakeDepthScale_SetBit(void* Obj);
	static const UECodeGen_Private::FBoolPropertyParams NewProp_FakeDepthScale;
	static const UECodeGen_Private::FFloatPropertyParams NewProp_FakeDepthScaleDistance;
	static void NewProp_ShowDebugSystemInWorld_SetBit(void* Obj);
	static const UECodeGen_Private::FBoolPropertyParams NewProp_ShowDebugSystemInWorld;
	static void NewProp_PassDynamicParametersFromRibbon_SetBit(void* Obj);
	static const UECodeGen_Private::FBoolPropertyParams NewProp_PassDynamicParametersFromRibbon;
	static void NewProp_DisableWarnings_SetBit(void* Obj);
	static const UECodeGen_Private::FBoolPropertyParams NewProp_DisableWarnings;
	static void NewProp_RestartSimulationOnPropertyChange_SetBit(void* Obj);
	static const UECodeGen_Private::FBoolPropertyParams NewProp_RestartSimulationOnPropertyChange;
	static const UECodeGen_Private::FObjectPropertyParams NewProp_NiagaraActor;
	static const UECodeGen_Private::FObjectPropertyParams NewProp_NiagaraComponent;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
	static UObject* (*const DependentSingletons[])();
	static constexpr FClassFunctionLinkInfo FuncInfo[] = {
		{ &Z_Construct_UFunction_UNiagaraSystemWidget_ActivateSystem, "ActivateSystem" }, // 3740975795
		{ &Z_Construct_UFunction_UNiagaraSystemWidget_DeactivateSystem, "DeactivateSystem" }, // 1759015262
		{ &Z_Construct_UFunction_UNiagaraSystemWidget_GetNiagaraComponent, "GetNiagaraComponent" }, // 2752915433
		{ &Z_Construct_UFunction_UNiagaraSystemWidget_GetRemapMaterial, "GetRemapMaterial" }, // 4041223328
		{ &Z_Construct_UFunction_UNiagaraSystemWidget_SetColorAndOpacity, "SetColorAndOpacity" }, // 1049850294
		{ &Z_Construct_UFunction_UNiagaraSystemWidget_SetDesiredWidgetSize, "SetDesiredWidgetSize" }, // 2096955682
		{ &Z_Construct_UFunction_UNiagaraSystemWidget_SetRemapMaterial, "SetRemapMaterial" }, // 2689415796
		{ &Z_Construct_UFunction_UNiagaraSystemWidget_UpdateNiagaraSystemReference, "UpdateNiagaraSystemReference" }, // 3943926695
		{ &Z_Construct_UFunction_UNiagaraSystemWidget_UpdateTickWhenPaused, "UpdateTickWhenPaused" }, // 3082534958
	};
	static_assert(UE_ARRAY_COUNT(FuncInfo) < 2048);
	static constexpr FCppClassTypeInfoStatic StaticCppClassTypeInfo = {
		TCppClassTypeTraits<UNiagaraSystemWidget>::IsAbstract,
	};
	static const UECodeGen_Private::FClassParams ClassParams;
};
const UECodeGen_Private::FObjectPropertyParams Z_Construct_UClass_UNiagaraSystemWidget_Statics::NewProp_NiagaraSystemReference = { "NiagaraSystemReference", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UNiagaraSystemWidget, NiagaraSystemReference), Z_Construct_UClass_UNiagaraSystem_NoRegister, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_NiagaraSystemReference_MetaData), NewProp_NiagaraSystemReference_MetaData) };
const UECodeGen_Private::FObjectPropertyParams Z_Construct_UClass_UNiagaraSystemWidget_Statics::NewProp_MaterialRemapList_ValueProp = { "MaterialRemapList", nullptr, (EPropertyFlags)0x0000000000000001, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, 1, Z_Construct_UClass_UMaterialInterface_NoRegister, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FObjectPropertyParams Z_Construct_UClass_UNiagaraSystemWidget_Statics::NewProp_MaterialRemapList_Key_KeyProp = { "MaterialRemapList_Key", nullptr, (EPropertyFlags)0x0004000000000001, UECodeGen_Private::EPropertyGenFlags::Object | UECodeGen_Private::EPropertyGenFlags::ObjectPtr, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, 0, Z_Construct_UClass_UMaterialInterface_NoRegister, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FMapPropertyParams Z_Construct_UClass_UNiagaraSystemWidget_Statics::NewProp_MaterialRemapList = { "MaterialRemapList", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Map, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UNiagaraSystemWidget, MaterialRemapList), EMapPropertyFlags::None, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_MaterialRemapList_MetaData), NewProp_MaterialRemapList_MetaData) };
void Z_Construct_UClass_UNiagaraSystemWidget_Statics::NewProp_AutoActivate_SetBit(void* Obj)
{
	((UNiagaraSystemWidget*)Obj)->AutoActivate = 1;
}
const UECodeGen_Private::FBoolPropertyParams Z_Construct_UClass_UNiagaraSystemWidget_Statics::NewProp_AutoActivate = { "AutoActivate", nullptr, (EPropertyFlags)0x0010000000000015, UECodeGen_Private::EPropertyGenFlags::Bool | UECodeGen_Private::EPropertyGenFlags::NativeBool, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, sizeof(bool), sizeof(UNiagaraSystemWidget), &Z_Construct_UClass_UNiagaraSystemWidget_Statics::NewProp_AutoActivate_SetBit, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_AutoActivate_MetaData), NewProp_AutoActivate_MetaData) };
void Z_Construct_UClass_UNiagaraSystemWidget_Statics::NewProp_TickWhenPaused_SetBit(void* Obj)
{
	((UNiagaraSystemWidget*)Obj)->TickWhenPaused = 1;
}
const UECodeGen_Private::FBoolPropertyParams Z_Construct_UClass_UNiagaraSystemWidget_Statics::NewProp_TickWhenPaused = { "TickWhenPaused", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Bool | UECodeGen_Private::EPropertyGenFlags::NativeBool, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, sizeof(bool), sizeof(UNiagaraSystemWidget), &Z_Construct_UClass_UNiagaraSystemWidget_Statics::NewProp_TickWhenPaused_SetBit, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_TickWhenPaused_MetaData), NewProp_TickWhenPaused_MetaData) };
const UECodeGen_Private::FStructPropertyParams Z_Construct_UClass_UNiagaraSystemWidget_Statics::NewProp_DesiredWidgetSize = { "DesiredWidgetSize", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, &UNiagaraSystemWidget::SetDesiredWidgetSize_WrapperImpl, &UNiagaraSystemWidget::GetDesiredWidgetSize_WrapperImpl, 1, STRUCT_OFFSET(UNiagaraSystemWidget, DesiredWidgetSize), Z_Construct_UScriptStruct_FVector2D, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_DesiredWidgetSize_MetaData), NewProp_DesiredWidgetSize_MetaData) };
const UECodeGen_Private::FStructPropertyParams Z_Construct_UClass_UNiagaraSystemWidget_Statics::NewProp_ColorAndOpacity = { "ColorAndOpacity", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, &UNiagaraSystemWidget::SetColorAndOpacity_WrapperImpl, &UNiagaraSystemWidget::GetColorAndOpacity_WrapperImpl, 1, STRUCT_OFFSET(UNiagaraSystemWidget, ColorAndOpacity), Z_Construct_UScriptStruct_FLinearColor, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_ColorAndOpacity_MetaData), NewProp_ColorAndOpacity_MetaData) };
void Z_Construct_UClass_UNiagaraSystemWidget_Statics::NewProp_FakeDepthScale_SetBit(void* Obj)
{
	((UNiagaraSystemWidget*)Obj)->FakeDepthScale = 1;
}
const UECodeGen_Private::FBoolPropertyParams Z_Construct_UClass_UNiagaraSystemWidget_Statics::NewProp_FakeDepthScale = { "FakeDepthScale", nullptr, (EPropertyFlags)0x0010040000000015, UECodeGen_Private::EPropertyGenFlags::Bool | UECodeGen_Private::EPropertyGenFlags::NativeBool, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, sizeof(bool), sizeof(UNiagaraSystemWidget), &Z_Construct_UClass_UNiagaraSystemWidget_Statics::NewProp_FakeDepthScale_SetBit, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_FakeDepthScale_MetaData), NewProp_FakeDepthScale_MetaData) };
const UECodeGen_Private::FFloatPropertyParams Z_Construct_UClass_UNiagaraSystemWidget_Statics::NewProp_FakeDepthScaleDistance = { "FakeDepthScaleDistance", nullptr, (EPropertyFlags)0x0010040000000015, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UNiagaraSystemWidget, FakeDepthScaleDistance), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_FakeDepthScaleDistance_MetaData), NewProp_FakeDepthScaleDistance_MetaData) };
void Z_Construct_UClass_UNiagaraSystemWidget_Statics::NewProp_ShowDebugSystemInWorld_SetBit(void* Obj)
{
	((UNiagaraSystemWidget*)Obj)->ShowDebugSystemInWorld = 1;
}
const UECodeGen_Private::FBoolPropertyParams Z_Construct_UClass_UNiagaraSystemWidget_Statics::NewProp_ShowDebugSystemInWorld = { "ShowDebugSystemInWorld", nullptr, (EPropertyFlags)0x0010040000000015, UECodeGen_Private::EPropertyGenFlags::Bool | UECodeGen_Private::EPropertyGenFlags::NativeBool, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, sizeof(bool), sizeof(UNiagaraSystemWidget), &Z_Construct_UClass_UNiagaraSystemWidget_Statics::NewProp_ShowDebugSystemInWorld_SetBit, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_ShowDebugSystemInWorld_MetaData), NewProp_ShowDebugSystemInWorld_MetaData) };
void Z_Construct_UClass_UNiagaraSystemWidget_Statics::NewProp_PassDynamicParametersFromRibbon_SetBit(void* Obj)
{
	((UNiagaraSystemWidget*)Obj)->PassDynamicParametersFromRibbon = 1;
}
const UECodeGen_Private::FBoolPropertyParams Z_Construct_UClass_UNiagaraSystemWidget_Statics::NewProp_PassDynamicParametersFromRibbon = { "PassDynamicParametersFromRibbon", nullptr, (EPropertyFlags)0x0010040000000015, UECodeGen_Private::EPropertyGenFlags::Bool | UECodeGen_Private::EPropertyGenFlags::NativeBool, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, sizeof(bool), sizeof(UNiagaraSystemWidget), &Z_Construct_UClass_UNiagaraSystemWidget_Statics::NewProp_PassDynamicParametersFromRibbon_SetBit, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_PassDynamicParametersFromRibbon_MetaData), NewProp_PassDynamicParametersFromRibbon_MetaData) };
void Z_Construct_UClass_UNiagaraSystemWidget_Statics::NewProp_DisableWarnings_SetBit(void* Obj)
{
	((UNiagaraSystemWidget*)Obj)->DisableWarnings = 1;
}
const UECodeGen_Private::FBoolPropertyParams Z_Construct_UClass_UNiagaraSystemWidget_Statics::NewProp_DisableWarnings = { "DisableWarnings", nullptr, (EPropertyFlags)0x0010040000000015, UECodeGen_Private::EPropertyGenFlags::Bool | UECodeGen_Private::EPropertyGenFlags::NativeBool, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, sizeof(bool), sizeof(UNiagaraSystemWidget), &Z_Construct_UClass_UNiagaraSystemWidget_Statics::NewProp_DisableWarnings_SetBit, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_DisableWarnings_MetaData), NewProp_DisableWarnings_MetaData) };
void Z_Construct_UClass_UNiagaraSystemWidget_Statics::NewProp_RestartSimulationOnPropertyChange_SetBit(void* Obj)
{
	((UNiagaraSystemWidget*)Obj)->RestartSimulationOnPropertyChange = 1;
}
const UECodeGen_Private::FBoolPropertyParams Z_Construct_UClass_UNiagaraSystemWidget_Statics::NewProp_RestartSimulationOnPropertyChange = { "RestartSimulationOnPropertyChange", nullptr, (EPropertyFlags)0x0010040000000015, UECodeGen_Private::EPropertyGenFlags::Bool | UECodeGen_Private::EPropertyGenFlags::NativeBool, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, sizeof(bool), sizeof(UNiagaraSystemWidget), &Z_Construct_UClass_UNiagaraSystemWidget_Statics::NewProp_RestartSimulationOnPropertyChange_SetBit, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_RestartSimulationOnPropertyChange_MetaData), NewProp_RestartSimulationOnPropertyChange_MetaData) };
const UECodeGen_Private::FObjectPropertyParams Z_Construct_UClass_UNiagaraSystemWidget_Statics::NewProp_NiagaraActor = { "NiagaraActor", nullptr, (EPropertyFlags)0x0040000000000000, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UNiagaraSystemWidget, NiagaraActor), Z_Construct_UClass_ANiagaraUIActor_NoRegister, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_NiagaraActor_MetaData), NewProp_NiagaraActor_MetaData) };
const UECodeGen_Private::FObjectPropertyParams Z_Construct_UClass_UNiagaraSystemWidget_Statics::NewProp_NiagaraComponent = { "NiagaraComponent", nullptr, (EPropertyFlags)0x0040000000080008, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UNiagaraSystemWidget, NiagaraComponent), Z_Construct_UClass_UNiagaraUIComponent_NoRegister, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_NiagaraComponent_MetaData), NewProp_NiagaraComponent_MetaData) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UClass_UNiagaraSystemWidget_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UNiagaraSystemWidget_Statics::NewProp_NiagaraSystemReference,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UNiagaraSystemWidget_Statics::NewProp_MaterialRemapList_ValueProp,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UNiagaraSystemWidget_Statics::NewProp_MaterialRemapList_Key_KeyProp,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UNiagaraSystemWidget_Statics::NewProp_MaterialRemapList,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UNiagaraSystemWidget_Statics::NewProp_AutoActivate,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UNiagaraSystemWidget_Statics::NewProp_TickWhenPaused,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UNiagaraSystemWidget_Statics::NewProp_DesiredWidgetSize,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UNiagaraSystemWidget_Statics::NewProp_ColorAndOpacity,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UNiagaraSystemWidget_Statics::NewProp_FakeDepthScale,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UNiagaraSystemWidget_Statics::NewProp_FakeDepthScaleDistance,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UNiagaraSystemWidget_Statics::NewProp_ShowDebugSystemInWorld,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UNiagaraSystemWidget_Statics::NewProp_PassDynamicParametersFromRibbon,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UNiagaraSystemWidget_Statics::NewProp_DisableWarnings,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UNiagaraSystemWidget_Statics::NewProp_RestartSimulationOnPropertyChange,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UNiagaraSystemWidget_Statics::NewProp_NiagaraActor,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UNiagaraSystemWidget_Statics::NewProp_NiagaraComponent,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UClass_UNiagaraSystemWidget_Statics::PropPointers) < 2048);
UObject* (*const Z_Construct_UClass_UNiagaraSystemWidget_Statics::DependentSingletons[])() = {
	(UObject* (*)())Z_Construct_UClass_UWidget,
	(UObject* (*)())Z_Construct_UPackage__Script_NiagaraUIRenderer,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UClass_UNiagaraSystemWidget_Statics::DependentSingletons) < 16);
const UECodeGen_Private::FClassParams Z_Construct_UClass_UNiagaraSystemWidget_Statics::ClassParams = {
	&UNiagaraSystemWidget::StaticClass,
	nullptr,
	&StaticCppClassTypeInfo,
	DependentSingletons,
	FuncInfo,
	Z_Construct_UClass_UNiagaraSystemWidget_Statics::PropPointers,
	nullptr,
	UE_ARRAY_COUNT(DependentSingletons),
	UE_ARRAY_COUNT(FuncInfo),
	UE_ARRAY_COUNT(Z_Construct_UClass_UNiagaraSystemWidget_Statics::PropPointers),
	0,
	0x00B000A0u,
	METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UClass_UNiagaraSystemWidget_Statics::Class_MetaDataParams), Z_Construct_UClass_UNiagaraSystemWidget_Statics::Class_MetaDataParams)
};
UClass* Z_Construct_UClass_UNiagaraSystemWidget()
{
	if (!Z_Registration_Info_UClass_UNiagaraSystemWidget.OuterSingleton)
	{
		UECodeGen_Private::ConstructUClass(Z_Registration_Info_UClass_UNiagaraSystemWidget.OuterSingleton, Z_Construct_UClass_UNiagaraSystemWidget_Statics::ClassParams);
	}
	return Z_Registration_Info_UClass_UNiagaraSystemWidget.OuterSingleton;
}
template<> NIAGARAUIRENDERER_API UClass* StaticClass<UNiagaraSystemWidget>()
{
	return UNiagaraSystemWidget::StaticClass();
}
DEFINE_VTABLE_PTR_HELPER_CTOR(UNiagaraSystemWidget);
UNiagaraSystemWidget::~UNiagaraSystemWidget() {}
// End Class UNiagaraSystemWidget

// Begin Registration
struct Z_CompiledInDeferFile_FID_Users_jakek_Documents_Unreal_Projects_Bristle54_5_5_plugins_NiagaraUIRenderer_Source_NiagaraUIRenderer_Public_NiagaraSystemWidget_h_Statics
{
	static constexpr FClassRegisterCompiledInInfo ClassInfo[] = {
		{ Z_Construct_UClass_UNiagaraSystemWidget, UNiagaraSystemWidget::StaticClass, TEXT("UNiagaraSystemWidget"), &Z_Registration_Info_UClass_UNiagaraSystemWidget, CONSTRUCT_RELOAD_VERSION_INFO(FClassReloadVersionInfo, sizeof(UNiagaraSystemWidget), 3625849867U) },
	};
};
static FRegisterCompiledInInfo Z_CompiledInDeferFile_FID_Users_jakek_Documents_Unreal_Projects_Bristle54_5_5_plugins_NiagaraUIRenderer_Source_NiagaraUIRenderer_Public_NiagaraSystemWidget_h_3604300338(TEXT("/Script/NiagaraUIRenderer"),
	Z_CompiledInDeferFile_FID_Users_jakek_Documents_Unreal_Projects_Bristle54_5_5_plugins_NiagaraUIRenderer_Source_NiagaraUIRenderer_Public_NiagaraSystemWidget_h_Statics::ClassInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_Users_jakek_Documents_Unreal_Projects_Bristle54_5_5_plugins_NiagaraUIRenderer_Source_NiagaraUIRenderer_Public_NiagaraSystemWidget_h_Statics::ClassInfo),
	nullptr, 0,
	nullptr, 0);
// End Registration
PRAGMA_ENABLE_DEPRECATION_WARNINGS
