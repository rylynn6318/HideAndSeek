#include "HSGameData.h"
#include "HSAssetManager.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(HSGameData)

UHSGameData::UHSGameData()
{
}

const UHSGameData& UHSGameData::UHSGameData::Get()
{
	return UHSAssetManager::Get().GetGameData();
}
