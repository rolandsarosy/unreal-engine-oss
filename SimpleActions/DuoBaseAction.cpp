#include "DuoGame/Public/GameplayAbilities/DuoBaseAction.h"

#include "Components/DuoActionComponent.h"
#include "Net/UnrealNetwork.h"
#include "GameplayTagContainer.h"
#include "DuoGame/DuoGame.h"

void UDuoBaseAction::OnActionAdded_Implementation(AActor* Instigator)
{
	LogOnScreen(this, FString::Printf(TEXT("Added: %s"), *Tag.ToString()), FColor::Green, 1.0F); // Should be enabled for Action debugging.
	ReplicationData.Instigator = Instigator;
	// We really can't know whether the action is running at this point or not, so we're not updating that part of the struct.
}

void UDuoBaseAction::OnActionRemoved_Implementation(AActor* Instigator)
{
	LogOnScreen(this, FString::Printf(TEXT("Removed: %s"), *Tag.ToString()), FColor::Green, 1.0F); // Should be enabled for Action debugging.
	ReplicationData.Instigator = Instigator;
	ReplicationData.bIsRunning = false; // Always false at this point because to be able to remove an Action, it must be stopped beforehand, or by the ActionComponent.
}

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

int32 UDuoBaseAction::GetFunctionCallspace(UFunction* Function, FFrame* Stack)
{
	if (UDuoActionComponent* Comp = GetOwningComponent()) return Comp->GetFunctionCallspace(Function, Stack);
	return Super::GetFunctionCallspace(Function, Stack);
}

bool UDuoBaseAction::CallRemoteFunction(UFunction* Function, void* Parms, FOutParmRec* OutParms, FFrame* Stack)
{
	if (const UDuoActionComponent* Component = GetOwningComponent())
	{
		if (AActor* Owner = Component->GetOwner())
		{
			if (UNetDriver* NetDriver = Owner->GetNetDriver())
			{
				NetDriver->ProcessRemoteFunction(Owner, Function, Parms, OutParms, Stack, this);
				return true;
			}
		}
	}

	return false;
}

bool UDuoBaseAction::IsRunning() const { return ReplicationData.bIsRunning; }

UDuoActionComponent* UDuoBaseAction::GetOwningComponent() const { return Cast<UDuoActionComponent>(GetOuter()); }

void UDuoBaseAction::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	UObject::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UDuoBaseAction, ReplicationData);
	DOREPLIFETIME(UDuoBaseAction, TimeStarted);
}
