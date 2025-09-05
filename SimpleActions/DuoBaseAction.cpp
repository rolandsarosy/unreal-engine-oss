#include "DuoGame/Public/GameplayAbilities/DuoBaseAction.h"

#include "Components/DuoActionComponent.h"
#include "Net/UnrealNetwork.h"
#include "GameplayTagContainer.h"
#include "DuoGame/DuoGame.h"

void UDuoBaseAction::StartAction_Implementation(AActor* Instigator)
{
	LogOnScreen(this, FString::Printf(TEXT("Started: %s"), *Tag.ToString()), FColor::Green, 1.0F); // Should be enabled for Action debugging.

	GetOwningComponent()->ActiveGameplayTags.AppendTags(GrantsTags);
	GetOwningComponent()->OnActionStarted.Broadcast(this, Instigator);
	ReplicationData.Instigator = Instigator;
	ReplicationData.bIsRunning = true;

	if (GetOwningComponent()->GetOwner()->HasAuthority()) TimeStarted = GetWorld()->TimeSeconds; // TimeStarted should only come from a trusted source.
}

void UDuoBaseAction::StopAction_Implementation(AActor* Instigator)
{
	LogOnScreen(this, FString::Printf(TEXT("Stopped: %s"), *Tag.ToString()), FColor::Black, 1.0F); // Should be enabled for Action debugging.

	GetOwningComponent()->ActiveGameplayTags.RemoveTags(GrantsTags);
	GetOwningComponent()->OnActionStopped.Broadcast(this, Instigator);
	ReplicationData.Instigator = Instigator;
	ReplicationData.bIsRunning = false;
}

bool UDuoBaseAction::CanStart_Implementation(AActor* Instigator)
{
	return !ReplicationData.bIsRunning && !GetOwningComponent()->ActiveGameplayTags.HasAny(BlockedTags);
}

UWorld* UDuoBaseAction::GetWorld() const
{
	// Outer is set when creating a new action via NewObject<T>.
	if (const UDuoActionComponent* ActorComponent = Cast<UDuoActionComponent>(GetOuter()))
	{
		return ActorComponent->GetWorld();
	}
	return nullptr;
}

void UDuoBaseAction::OnRep_ReplicationData()
{
	if (IsRunning())
	{
		StartAction(ReplicationData.Instigator);
	}
	else
	{
		StopAction(ReplicationData.Instigator);
	}
}

bool UDuoBaseAction::IsRunning() const { return ReplicationData.bIsRunning; }

UDuoActionComponent* UDuoBaseAction::GetOwningComponent() const { return Cast<UDuoActionComponent>(GetOuter()); }

void UDuoBaseAction::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	UObject::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UDuoBaseAction, ReplicationData);
	DOREPLIFETIME(UDuoBaseAction, TimeStarted);
}
