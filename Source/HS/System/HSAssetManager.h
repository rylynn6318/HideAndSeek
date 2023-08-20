

#pragma once

#include "CoreMinimal.h"
#include "Engine/AssetManager.h"
#include "HSAssetManager.generated.h"

/**
 * 
 */
UCLASS()
class HS_API UHSAssetManager : public UAssetManager
{
	GENERATED_BODY()
	
public:
	virtual void StartInitialLoading() override;
};
