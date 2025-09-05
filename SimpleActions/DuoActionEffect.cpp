#include "DuoGame/Public/GameplayAbilities/DuoActionEffect.h"

#include "Components/DuoActionComponent.h"
#include "GameFramework/GameStateBase.h"

UDuoActionEffect::UDuoActionEffect()
{
	bAutoStart = true;
	Duration = 3.0f;
	Period = 0.25f;
}

void UDuoActionEffect::StartAction_Implementation(AActor* Instigator)
{
	Super::StartAction_Implementation(Instigator);

	if (Duration > 0.0f)
	{
		GetWorld()->GetTimerManager().SetTimer(DurationHandle, [this, Instigator] { StopAction(Instigator); }, Duration, false);
	}

	if (Period > 0.0f)
	{
		GetWorld()->GetTimerManager().SetTimer(PeriodHandle, [this, Instigator] { ExecutePeriodicEffect(Instigator); }, Period, true);
	}
}

void UDuoActionEffect::StopAction_Implementation(AActor* Instigator)
{
	// This handles cases where the last 'tick' of the effect would happen at almost the same time as the stopping of the action.
	if (GetWorld()->GetTimerManager().GetTimerRemaining(PeriodHandle) < KINDA_SMALL_NUMBER) ExecutePeriodicEffect(Instigator);

	Super::StopAction_Implementation(Instigator);
	GetWorld()->GetTimerManager().ClearTimer(DurationHandle);
	GetWorld()->GetTimerManager().ClearTimer(PeriodHandle);

	if (UDuoActionComponent* ActionComponent = GetOwningComponent()) ActionComponent->RemoveAction(this, Instigator);
}

float UDuoActionEffect::GetTimeRemaining() const
{
	const AGameStateBase* GameState = GetWorld()->GetGameState<AGameStateBase>();
	if (!ensure(GameState)) return Duration;

	return TimeStarted + Duration - GameState->GetServerWorldTimeSeconds();
}
