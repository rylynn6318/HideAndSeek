#pragma once

#include "Logging/LogMacros.h"

class UObject;

HS_API DECLARE_LOG_CATEGORY_EXTERN(LogHS, Log, All);
HS_API DECLARE_LOG_CATEGORY_EXTERN(LogHSExperience, Log, All);
HS_API DECLARE_LOG_CATEGORY_EXTERN(LogHSAbilitySystem, Log, All);
HS_API DECLARE_LOG_CATEGORY_EXTERN(LogHSTeams, Log, All);

HS_API FString GetClientServerContextString(UObject* ContextObject = nullptr);
