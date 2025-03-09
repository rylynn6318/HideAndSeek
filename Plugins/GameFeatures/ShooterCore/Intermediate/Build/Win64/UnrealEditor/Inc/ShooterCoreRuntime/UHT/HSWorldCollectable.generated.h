// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

// IWYU pragma: private, include "HSWorldCollectable.h"
#include "UObject/ObjectMacros.h"
#include "UObject/ScriptMacros.h"

PRAGMA_DISABLE_DEPRECATION_WARNINGS
#ifdef SHOOTERCORERUNTIME_HSWorldCollectable_generated_h
#error "HSWorldCollectable.generated.h already included, missing '#pragma once' in HSWorldCollectable.h"
#endif
#define SHOOTERCORERUNTIME_HSWorldCollectable_generated_h

#define FID_HideAndSeek_Plugins_GameFeatures_ShooterCore_Source_ShooterCoreRuntime_Public_HSWorldCollectable_h_21_INCLASS_NO_PURE_DECLS \
private: \
	static void StaticRegisterNativesAHSWorldCollectable(); \
	friend struct Z_Construct_UClass_AHSWorldCollectable_Statics; \
public: \
	DECLARE_CLASS(AHSWorldCollectable, AActor, COMPILED_IN_FLAGS(CLASS_Abstract | CLASS_Config), CASTCLASS_None, TEXT("/Script/ShooterCoreRuntime"), NO_API) \
	DECLARE_SERIALIZER(AHSWorldCollectable) \
	virtual UObject* _getUObject() const override { return const_cast<AHSWorldCollectable*>(this); }


#define FID_HideAndSeek_Plugins_GameFeatures_ShooterCore_Source_ShooterCoreRuntime_Public_HSWorldCollectable_h_21_ENHANCED_CONSTRUCTORS \
private: \
	/** Private move- and copy-constructors, should never be used */ \
	AHSWorldCollectable(AHSWorldCollectable&&); \
	AHSWorldCollectable(const AHSWorldCollectable&); \
public: \
	DECLARE_VTABLE_PTR_HELPER_CTOR(NO_API, AHSWorldCollectable); \
	DEFINE_VTABLE_PTR_HELPER_CTOR_CALLER(AHSWorldCollectable); \
	DEFINE_ABSTRACT_DEFAULT_CONSTRUCTOR_CALL(AHSWorldCollectable) \
	NO_API virtual ~AHSWorldCollectable();


#define FID_HideAndSeek_Plugins_GameFeatures_ShooterCore_Source_ShooterCoreRuntime_Public_HSWorldCollectable_h_18_PROLOG
#define FID_HideAndSeek_Plugins_GameFeatures_ShooterCore_Source_ShooterCoreRuntime_Public_HSWorldCollectable_h_21_GENERATED_BODY \
PRAGMA_DISABLE_DEPRECATION_WARNINGS \
public: \
	FID_HideAndSeek_Plugins_GameFeatures_ShooterCore_Source_ShooterCoreRuntime_Public_HSWorldCollectable_h_21_INCLASS_NO_PURE_DECLS \
	FID_HideAndSeek_Plugins_GameFeatures_ShooterCore_Source_ShooterCoreRuntime_Public_HSWorldCollectable_h_21_ENHANCED_CONSTRUCTORS \
private: \
PRAGMA_ENABLE_DEPRECATION_WARNINGS


template<> SHOOTERCORERUNTIME_API UClass* StaticClass<class AHSWorldCollectable>();

#undef CURRENT_FILE_ID
#define CURRENT_FILE_ID FID_HideAndSeek_Plugins_GameFeatures_ShooterCore_Source_ShooterCoreRuntime_Public_HSWorldCollectable_h


PRAGMA_ENABLE_DEPRECATION_WARNINGS
