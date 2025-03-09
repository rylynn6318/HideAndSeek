// Copyright Epic Games, Inc. All Rights Reserved.

#include "MessageProcessors/AssistProcessor.h"

#include "GameFramework/PlayerState.h"
#include "Messages/HSVerbMessage.h"
#include "Messages/HSVerbMessageHelpers.h"
#include "NativeGameplayTags.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(AssistProcessor)

UE_DEFINE_GAMEPLAY_TAG_STATIC(TAG_HS_Elimination_Message, "HS.Elimination.Message");
UE_DEFINE_GAMEPLAY_TAG_STATIC(TAG_HS_Damage_Message, "HS.Damage.Message");
UE_DEFINE_GAMEPLAY_TAG_STATIC(TAG_HS_Assist_Message, "HS.Assist.Message");

void UAssistProcessor::StartListening()
{
	UGameplayMessageSubsystem& MessageSubsystem = UGameplayMessageSubsystem::Get(this);
	AddListenerHandle(MessageSubsystem.RegisterListener(TAG_HS_Elimination_Message, this, &ThisClass::OnEliminationMessage));
	AddListenerHandle(MessageSubsystem.RegisterListener(TAG_HS_Damage_Message, this, &ThisClass::OnDamageMessage));
}

void UAssistProcessor::OnDamageMessage(FGameplayTag Channel, const FHSVerbMessage& Payload)
{
	if (Payload.Instigator != Payload.Target)
	{
		if (APlayerState* InstigatorPS = UHSVerbMessageHelpers::GetPlayerStateFromObject(Payload.Instigator))
		{
			if (APlayerState* TargetPS = UHSVerbMessageHelpers::GetPlayerStateFromObject(Payload.Target))
			{
				FPlayerAssistDamageTracking& Damage = DamageHistory.FindOrAdd(TargetPS);
				float& DamageTotalFromTarget = Damage.AccumulatedDamageByPlayer.FindOrAdd(InstigatorPS);
				DamageTotalFromTarget += Payload.Magnitude;
			}
		}
	}
}


void UAssistProcessor::OnEliminationMessage(FGameplayTag Channel, const FHSVerbMessage& Payload)
{
	if (APlayerState* TargetPS = Cast<APlayerState>(Payload.Target))
	{
		// Grant an assist to each player who damaged the target but wasn't the instigator
		if (FPlayerAssistDamageTracking* DamageOnTarget = DamageHistory.Find(TargetPS))
		{
			for (const auto& KVP : DamageOnTarget->AccumulatedDamageByPlayer)
			{
				if (APlayerState* AssistPS = KVP.Key)
				{
					if (AssistPS != Payload.Instigator)
					{
						FHSVerbMessage AssistMessage;
						AssistMessage.Verb = TAG_HS_Assist_Message;
						AssistMessage.Instigator = AssistPS;
						//@TODO: Get default tags from a player state or save off most recent tags during assist damage?
						//AssistMessage.InstigatorTags = ;
						AssistMessage.Target = TargetPS;
						AssistMessage.TargetTags = Payload.TargetTags;
						AssistMessage.ContextTags = Payload.ContextTags;
						AssistMessage.Magnitude = KVP.Value;

						UGameplayMessageSubsystem& MessageSubsystem = UGameplayMessageSubsystem::Get(this);
						MessageSubsystem.BroadcastMessage(AssistMessage.Verb, AssistMessage);
					}
				}
			}

			// Clear the damage log for the eliminated player
			DamageHistory.Remove(TargetPS);
		}
	}
}

