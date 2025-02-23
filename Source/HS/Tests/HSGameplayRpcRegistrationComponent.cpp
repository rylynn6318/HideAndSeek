// Copyright Epic Games, Inc. All Rights Reserved.

#include "HSGameplayRpcRegistrationComponent.h"
#include "Player/HSPlayerController.h"
#include "Character/HSPawn.h"
#include "Player/HSPlayerState.h"
#include "Engine/GameViewportClient.h"
#include "Engine/GameInstance.h"
#include "Misc/CommandLine.h"
#include "EngineMinimal.h"
#include "Character/HSHealthComponent.h"
//#include "Inventory/HSInventoryItemDefinition.h"
//#include "Inventory/HSInventoryItemInstance.h"
//#include "Inventory/HSInventoryManagerComponent.h"
#include "Character/HSPawnExtensionComponent.h"

UHSGameplayRpcRegistrationComponent* UHSGameplayRpcRegistrationComponent::ObjectInstance = nullptr;

UHSGameplayRpcRegistrationComponent* UHSGameplayRpcRegistrationComponent::GetInstance()
{
#if WITH_RPC_REGISTRY
	if (ObjectInstance == nullptr)
	{
		ObjectInstance = NewObject<UHSGameplayRpcRegistrationComponent>();
		FParse::Value(FCommandLine::Get(), TEXT("externalrpclistenaddress="), ObjectInstance->ListenerAddress);
		FParse::Value(FCommandLine::Get(), TEXT("rpcsenderid="), ObjectInstance->SenderID);
		if (!UExternalRpcRegistry::GetInstance())
		{
			GLog->Log(TEXT("BotRPC"), ELogVerbosity::Warning, FString::Printf(TEXT("Unable to create RPC Registry Instance. This might lead to issues using the RPC Registry.")));
		}
		ObjectInstance->AddToRoot();
	}
#endif
	return ObjectInstance;
}

UWorld* FindGameWorld()
{
	//Find Game World
	if (GEngine->GameViewport)
	{
		UGameInstance* GameInstance = GEngine->GameViewport->GetGameInstance();
		return GameInstance ? GameInstance->GetWorld() : nullptr;
	}
	return GWorld;
}

AHSPlayerController* GetPlayerController()
{
	UWorld* LocalWorld = FindGameWorld();
	if (!LocalWorld)
	{
		return nullptr;
	}
	//Find PlayerController
	AHSPlayerController* PlayerController = Cast<AHSPlayerController>(LocalWorld->GetFirstPlayerController());
	if (!PlayerController)
	{
		return nullptr;
	}
	else
	{
		return PlayerController;
	}
}

#if WITH_RPC_REGISTRY

TSharedPtr<FJsonObject> UHSGameplayRpcRegistrationComponent::GetJsonObjectFromRequestBody(TArray<uint8> InRequestBody)
{
	FUTF8ToTCHAR WByteBuffer(reinterpret_cast<const ANSICHAR*>(InRequestBody.GetData()), InRequestBody.Num());
	const FString IncomingRequestBody =  FString::ConstructFromPtrSize(WByteBuffer.Get(), WByteBuffer.Length());
	TSharedPtr<FJsonObject> BodyObject = MakeShareable(new FJsonObject());
	TSharedRef<TJsonReader<>> JsonReader = TJsonReaderFactory<>::Create(IncomingRequestBody);

	if (FJsonSerializer::Deserialize(JsonReader, BodyObject) && BodyObject.IsValid())
	{
		return BodyObject;
	}

	return nullptr;
}


void UHSGameplayRpcRegistrationComponent::RegisterAlwaysOnHttpCallbacks()
{
	Super::RegisterAlwaysOnHttpCallbacks();	
	const FExternalRpcArgumentDesc CommandDesc(TEXT("command"), TEXT("string"), TEXT("The command to tell the executable to run."));

	RegisterHttpCallback(FName(TEXT("CheatCommand")),
		FHttpPath("/core/cheatcommand"),
		EHttpServerRequestVerbs::VERB_POST,
		FHttpRequestHandler::CreateUObject(this, &ThisClass::HttpExecuteCheatCommand),
		true,
		TEXT("Cheats"),
		TEXT("raw"),
		{ CommandDesc });
}

void UHSGameplayRpcRegistrationComponent::RegisterInMatchHttpCallbacks()
{
	RegisterHttpCallback(FName(TEXT("GetPlayerStatus")),
		FHttpPath("/player/status"),
		EHttpServerRequestVerbs::VERB_GET,
		FHttpRequestHandler::CreateUObject(this, &ThisClass::HttpGetPlayerVitalsCommand),
		true);

	RegisterHttpCallback(FName(TEXT("PlayerFireOnce")),
		FHttpPath("/player/status"),
		EHttpServerRequestVerbs::VERB_POST,
		FHttpRequestHandler::CreateUObject(this, &ThisClass::HttpFireOnceCommand),
		true);
	
}

void UHSGameplayRpcRegistrationComponent::RegisterFrontendHttpCallbacks()
{
    // TODO: Add Matchmaking RPCs here
}


void UHSGameplayRpcRegistrationComponent::DeregisterHttpCallbacks()
{
	Super::DeregisterHttpCallbacks();
}

bool UHSGameplayRpcRegistrationComponent::HttpExecuteCheatCommand(const FHttpServerRequest& Request, const FHttpResultCallback& OnComplete)
{
	TSharedPtr<FJsonObject> BodyObject = GetJsonObjectFromRequestBody(Request.Body);

	if (!BodyObject.IsValid())
	{
		TUniquePtr<FHttpServerResponse>Response = CreateSimpleResponse(false, TEXT("Invalid body object"));
		OnComplete(MoveTemp(Response));
		return true;
	}
	if (BodyObject->GetStringField(TEXT("command")).IsEmpty())
	{
		TUniquePtr<FHttpServerResponse>Response = CreateSimpleResponse(false, TEXT("command not found in json body"));
		OnComplete(MoveTemp(Response));
		return true;
	}
	AHSPlayerController* LPC = GetPlayerController();
	if (!LPC)
	{
		TUniquePtr<FHttpServerResponse>Response = CreateSimpleResponse(false, TEXT("player controller not found"));
		OnComplete(MoveTemp(Response));
		return true;
	}
	FString CheatCommand = FString::Printf(TEXT("%s"), *BodyObject->GetStringField(TEXT("command")));
	LPC->ConsoleCommand(*CheatCommand, true);

	TUniquePtr<FHttpServerResponse>Response = CreateSimpleResponse(true);
	OnComplete(MoveTemp(Response));
	return true;
}

bool UHSGameplayRpcRegistrationComponent::HttpFireOnceCommand(const FHttpServerRequest& Request, const FHttpResultCallback& OnComplete)
{
	AHSPlayerController* LPC = GetPlayerController();
	if (!LPC)
	{
		TUniquePtr<FHttpServerResponse> Response = CreateSimpleResponse(false, TEXT("No player controller found"));
		OnComplete(MoveTemp(Response));
		return true;
	}
	APawn* FortPlayerPawn = LPC->GetPawn();
	if (!FortPlayerPawn)
	{
		TUniquePtr<FHttpServerResponse> Response = CreateSimpleResponse(false, TEXT("Player pawn not found"));
		OnComplete(MoveTemp(Response));
		return true;
	}

	// TODO: Fire Once here
	TUniquePtr<FHttpServerResponse> Response = CreateSimpleResponse(true);
	OnComplete(MoveTemp(Response));
	return true;
}

bool UHSGameplayRpcRegistrationComponent::HttpGetPlayerVitalsCommand(const FHttpServerRequest& Request, const FHttpResultCallback& OnComplete)
{
	//AHSPlayerController* LPC = GetPlayerController();
	//if (!LPC)
	//{
	//	TUniquePtr<FHttpServerResponse> Response = CreateSimpleResponse(false, TEXT("No player controller found"));
	//	OnComplete(MoveTemp(Response));
	//	return true;
	//}
	//APawn* PlayerPawn = LPC->GetPawn();
	//if (!PlayerPawn)
	//{
	//	TUniquePtr<FHttpServerResponse> Response = CreateSimpleResponse(false, TEXT("Player pawn not found"));
	//	OnComplete(MoveTemp(Response));
	//	return true;
	//}

	//AHSPlayerState* HSPlayerState = LPC->GetHSPlayerState();
	//if (!HSPlayerState)
	//{
	//	TUniquePtr<FHttpServerResponse> Response = CreateSimpleResponse(false, TEXT("Player state not found"));
	//	OnComplete(MoveTemp(Response));
	//	return true;
	//}

	//FString ResponseStr; 
	//TSharedRef<TJsonWriter<>> JsonWriter = TJsonWriterFactory<>::Create(&ResponseStr);
	//TSharedPtr<FJsonObject> BodyObject = MakeShareable(new FJsonObject());
	//JsonWriter->WriteObjectStart();
	//if (UHSHealthComponent* HealthComponent = UHSHealthComponent::FindHealthComponent(PlayerPawn))
	//{
	//	JsonWriter->WriteValue(TEXT("health"), FString::SanitizeFloat(HealthComponent->GetHealth()));
	//}
	//if (UHSInventoryManagerComponent* InventoryComponent = LPC->GetComponentByClass<UHSInventoryManagerComponent>())
	//{
	//	JsonWriter->WriteArrayStart(TEXT("inventory"));
	//	for (UHSInventoryItemInstance* ItemInstance : InventoryComponent->GetAllItems())
	//	{
	//		// TODO: Dump any relevant player info here.
	//	}
	//	JsonWriter->WriteArrayEnd();
	//}
	//JsonWriter->WriteObjectEnd();
	//JsonWriter->Close();
	//TUniquePtr<FHttpServerResponse>Response = FHttpServerResponse::Create(ResponseStr, TEXT("application/json"));
	//OnComplete(MoveTemp(Response));
	return true;
}

#endif