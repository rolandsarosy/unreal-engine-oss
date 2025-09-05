#pragma once

#include "CoreMinimal.h"

/**
 * Logs a message on the screen using the Unreal Engine's AddOnScreenDebugMessage function. Focus on Client/Server relationships for replication.
 *
 * @param WorldContext - The context within which the method is being called. It is an object derived from the UObject class.
 * @param Msg - The message string to be displayed on the screen.
 * @param Color - The color of the message text. It is an optional parameter with a default value of FColor::White.
 * @param Duration - The duration for which the message should be displayed on the screen. It is an optional parameter with a default value of 5.0f seconds.
 */
static void LogOnScreen(const UObject* WorldContext, const FString& Msg, const FColor Color = FColor::White, const float Duration = 5.0f)
{
	if (!ensure(WorldContext)) return;

	const UWorld* World = WorldContext->GetWorld();
	if (!ensure(World)) return;

	const FString NetPrefix = World->IsNetMode(NM_Client) ? "[CLIENT]" : "[SERVER]";
	if (GEngine) GEngine->AddOnScreenDebugMessage(-1, Duration, Color, NetPrefix + " " + Msg);
}
