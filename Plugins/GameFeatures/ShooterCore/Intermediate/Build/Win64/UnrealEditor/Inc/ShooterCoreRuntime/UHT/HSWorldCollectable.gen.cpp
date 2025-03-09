// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "ShooterCoreRuntime/Public/HSWorldCollectable.h"
#include "HS/Interaction/InteractionOption.h"
#include "HS/Inventory/IPickupable.h"
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodeHSWorldCollectable() {}

// Begin Cross Module References
ENGINE_API UClass* Z_Construct_UClass_AActor();
HS_API UClass* Z_Construct_UClass_UInteractableTarget_NoRegister();
HS_API UClass* Z_Construct_UClass_UPickupable_NoRegister();
HS_API UScriptStruct* Z_Construct_UScriptStruct_FInteractionOption();
HS_API UScriptStruct* Z_Construct_UScriptStruct_FInventoryPickup();
SHOOTERCORERUNTIME_API UClass* Z_Construct_UClass_AHSWorldCollectable();
SHOOTERCORERUNTIME_API UClass* Z_Construct_UClass_AHSWorldCollectable_NoRegister();
UPackage* Z_Construct_UPackage__Script_ShooterCoreRuntime();
// End Cross Module References

// Begin Class AHSWorldCollectable
void AHSWorldCollectable::StaticRegisterNativesAHSWorldCollectable()
{
}
IMPLEMENT_CLASS_NO_AUTO_REGISTRATION(AHSWorldCollectable);
UClass* Z_Construct_UClass_AHSWorldCollectable_NoRegister()
{
	return AHSWorldCollectable::StaticClass();
}
struct Z_Construct_UClass_AHSWorldCollectable_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Class_MetaDataParams[] = {
		{ "BlueprintType", "true" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "/**\n * \n */" },
#endif
		{ "IncludePath", "HSWorldCollectable.h" },
		{ "IsBlueprintBase", "true" },
		{ "ModuleRelativePath", "Public/HSWorldCollectable.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_Option_MetaData[] = {
		{ "Category", "HSWorldCollectable" },
		{ "ModuleRelativePath", "Public/HSWorldCollectable.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_StaticInventory_MetaData[] = {
		{ "Category", "HSWorldCollectable" },
		{ "ModuleRelativePath", "Public/HSWorldCollectable.h" },
	};
#endif // WITH_METADATA
	static const UECodeGen_Private::FStructPropertyParams NewProp_Option;
	static const UECodeGen_Private::FStructPropertyParams NewProp_StaticInventory;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
	static UObject* (*const DependentSingletons[])();
	static const UECodeGen_Private::FImplementedInterfaceParams InterfaceParams[];
	static constexpr FCppClassTypeInfoStatic StaticCppClassTypeInfo = {
		TCppClassTypeTraits<AHSWorldCollectable>::IsAbstract,
	};
	static const UECodeGen_Private::FClassParams ClassParams;
};
const UECodeGen_Private::FStructPropertyParams Z_Construct_UClass_AHSWorldCollectable_Statics::NewProp_Option = { "Option", nullptr, (EPropertyFlags)0x0020088000000001, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(AHSWorldCollectable, Option), Z_Construct_UScriptStruct_FInteractionOption, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_Option_MetaData), NewProp_Option_MetaData) }; // 940881538
const UECodeGen_Private::FStructPropertyParams Z_Construct_UClass_AHSWorldCollectable_Statics::NewProp_StaticInventory = { "StaticInventory", nullptr, (EPropertyFlags)0x0020080000000001, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(AHSWorldCollectable, StaticInventory), Z_Construct_UScriptStruct_FInventoryPickup, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_StaticInventory_MetaData), NewProp_StaticInventory_MetaData) }; // 248012086
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UClass_AHSWorldCollectable_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_AHSWorldCollectable_Statics::NewProp_Option,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_AHSWorldCollectable_Statics::NewProp_StaticInventory,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UClass_AHSWorldCollectable_Statics::PropPointers) < 2048);
UObject* (*const Z_Construct_UClass_AHSWorldCollectable_Statics::DependentSingletons[])() = {
	(UObject* (*)())Z_Construct_UClass_AActor,
	(UObject* (*)())Z_Construct_UPackage__Script_ShooterCoreRuntime,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UClass_AHSWorldCollectable_Statics::DependentSingletons) < 16);
const UECodeGen_Private::FImplementedInterfaceParams Z_Construct_UClass_AHSWorldCollectable_Statics::InterfaceParams[] = {
	{ Z_Construct_UClass_UInteractableTarget_NoRegister, (int32)VTABLE_OFFSET(AHSWorldCollectable, IInteractableTarget), false },  // 1039895530
	{ Z_Construct_UClass_UPickupable_NoRegister, (int32)VTABLE_OFFSET(AHSWorldCollectable, IPickupable), false },  // 633521740
};
const UECodeGen_Private::FClassParams Z_Construct_UClass_AHSWorldCollectable_Statics::ClassParams = {
	&AHSWorldCollectable::StaticClass,
	"Engine",
	&StaticCppClassTypeInfo,
	DependentSingletons,
	nullptr,
	Z_Construct_UClass_AHSWorldCollectable_Statics::PropPointers,
	InterfaceParams,
	UE_ARRAY_COUNT(DependentSingletons),
	0,
	UE_ARRAY_COUNT(Z_Construct_UClass_AHSWorldCollectable_Statics::PropPointers),
	UE_ARRAY_COUNT(InterfaceParams),
	0x008000A5u,
	METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UClass_AHSWorldCollectable_Statics::Class_MetaDataParams), Z_Construct_UClass_AHSWorldCollectable_Statics::Class_MetaDataParams)
};
UClass* Z_Construct_UClass_AHSWorldCollectable()
{
	if (!Z_Registration_Info_UClass_AHSWorldCollectable.OuterSingleton)
	{
		UECodeGen_Private::ConstructUClass(Z_Registration_Info_UClass_AHSWorldCollectable.OuterSingleton, Z_Construct_UClass_AHSWorldCollectable_Statics::ClassParams);
	}
	return Z_Registration_Info_UClass_AHSWorldCollectable.OuterSingleton;
}
template<> SHOOTERCORERUNTIME_API UClass* StaticClass<AHSWorldCollectable>()
{
	return AHSWorldCollectable::StaticClass();
}
DEFINE_VTABLE_PTR_HELPER_CTOR(AHSWorldCollectable);
AHSWorldCollectable::~AHSWorldCollectable() {}
// End Class AHSWorldCollectable

// Begin Registration
struct Z_CompiledInDeferFile_FID_HideAndSeek_Plugins_GameFeatures_ShooterCore_Source_ShooterCoreRuntime_Public_HSWorldCollectable_h_Statics
{
	static constexpr FClassRegisterCompiledInInfo ClassInfo[] = {
		{ Z_Construct_UClass_AHSWorldCollectable, AHSWorldCollectable::StaticClass, TEXT("AHSWorldCollectable"), &Z_Registration_Info_UClass_AHSWorldCollectable, CONSTRUCT_RELOAD_VERSION_INFO(FClassReloadVersionInfo, sizeof(AHSWorldCollectable), 4227274192U) },
	};
};
static FRegisterCompiledInInfo Z_CompiledInDeferFile_FID_HideAndSeek_Plugins_GameFeatures_ShooterCore_Source_ShooterCoreRuntime_Public_HSWorldCollectable_h_3477119815(TEXT("/Script/ShooterCoreRuntime"),
	Z_CompiledInDeferFile_FID_HideAndSeek_Plugins_GameFeatures_ShooterCore_Source_ShooterCoreRuntime_Public_HSWorldCollectable_h_Statics::ClassInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_HideAndSeek_Plugins_GameFeatures_ShooterCore_Source_ShooterCoreRuntime_Public_HSWorldCollectable_h_Statics::ClassInfo),
	nullptr, 0,
	nullptr, 0);
// End Registration
PRAGMA_ENABLE_DEPRECATION_WARNINGS
