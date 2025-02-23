#include "HSDeveloperSettings.h"
#include "Misc/App.h"
#include "Widgets/Notifications/SNotificationList.h"
#include "Framework/Notifications/NotificationManager.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(HSDeveloperSettings)

#define LOCTEXT_NAMESPACE "HSCheats"

UHSDeveloperSettings::UHSDeveloperSettings()
{
}

FName UHSDeveloperSettings::GetCategoryName() const
{
	return FApp::GetProjectName();
}

#if WITH_EDITOR
void UHSDeveloperSettings::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	ApplySettings();
}

void UHSDeveloperSettings::PostReloadConfig(FProperty* PropertyThatWasLoaded)
{
	Super::PostReloadConfig(PropertyThatWasLoaded);

	ApplySettings();
}

void UHSDeveloperSettings::PostInitProperties()
{
	Super::PostInitProperties();

	ApplySettings();
}

void UHSDeveloperSettings::ApplySettings()
{
}

void UHSDeveloperSettings::OnPlayInEditorStarted() const
{
	// Show a notification toast to remind the user that there's an experience override set
	if (ExperienceOverride.IsValid())
	{
		FNotificationInfo Info(FText::Format(
			LOCTEXT("ExperienceOverrideActive", "Developer Settings Override\nExperience {0}"),
			FText::FromName(ExperienceOverride.PrimaryAssetName)
		));
		Info.ExpireDuration = 2.0f;
		FSlateNotificationManager::Get().AddNotification(Info);
	}
}
#endif

#undef LOCTEXT_NAMESPACE

