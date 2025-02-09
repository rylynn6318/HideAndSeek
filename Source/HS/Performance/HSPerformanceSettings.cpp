#include "HSPerformanceSettings.h"

#include "Engine/PlatformSettingsManager.h"
#include "Misc/EnumRange.h"
#include "Performance/HSPerformanceStatTypes.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(HSPerformanceSettings)

//////////////////////////////////////////////////////////////////////

UHSPlatformSpecificRenderingSettings::UHSPlatformSpecificRenderingSettings()
{
	MobileFrameRateLimits.Append({ 20, 30, 45, 60, 90, 120 });
}

const UHSPlatformSpecificRenderingSettings* UHSPlatformSpecificRenderingSettings::Get()
{
	UHSPlatformSpecificRenderingSettings* Result = UPlatformSettingsManager::Get().GetSettingsForPlatform<ThisClass>();
	check(Result);
	return Result;
}

//////////////////////////////////////////////////////////////////////

UHSPerformanceSettings::UHSPerformanceSettings()
{
	PerPlatformSettings.Initialize(UHSPlatformSpecificRenderingSettings::StaticClass());

	CategoryName = TEXT("Game");

	DesktopFrameRateLimits.Append({ 30, 60, 120, 144, 160, 165, 180, 200, 240, 360 });

	// Default to all stats are allowed
	FHSPerformanceStatGroup& StatGroup = UserFacingPerformanceStats.AddDefaulted_GetRef();
	for (EHSDisplayablePerformanceStat PerfStat : TEnumRange<EHSDisplayablePerformanceStat>())
	{
		StatGroup.AllowedStats.Add(PerfStat);
	}
}

