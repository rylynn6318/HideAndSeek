#pragma once

#include "GauntletTestControllerBootTest.h"
#include "HSTestControllerBootTest.generated.h"

class UObject;

UCLASS()
class UHSTestControllerBootTest : public UGauntletTestControllerBootTest
{
	GENERATED_BODY()

protected:

	// This test needs a delay as the test can be over before focus is returned to Gauntlet after launching the game.
	// This can cause the test to be over before Gauntlet can even know that it is running and will cause the test to fail.
	//@TODO: Comment and delay copied from ShooterGame.  Still needed?
	const double TestDelay = 20.0;

	//~UGauntletTestControllerBootTest interface
	virtual bool IsBootProcessComplete() const override;
	//~End of UGauntletTestControllerBootTest interface
};
