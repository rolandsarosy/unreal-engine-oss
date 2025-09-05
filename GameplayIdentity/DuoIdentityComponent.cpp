#include "Components/DuoIdentityComponent.h"

#include "DuoGame/DuoGame.h"
#include "Net/UnrealNetwork.h"

UDuoIdentityComponent::UDuoIdentityComponent()
{
	SetIsReplicatedByDefault(true);

#if !UE_BUILD_SHIPPING
	// For non-shipping builds, we can leave ticking enabled for debugging.
	PrimaryComponentTick.bCanEverTick = false; 
#else
	// For the final SHIPPING build, we EXPLICITLY and ALWAYS disable ticking.
	PrimaryComponentTick.bCanEverTick = false;
#endif
}

void UDuoIdentityComponent::BeginPlay()
{
	Super::BeginPlay();

	if (GetOwner()->HasAuthority() && !DefaultIdentityGameplayTags.IsEmpty()) // Server only
	{
		for (const FGameplayTag Tag : DefaultIdentityGameplayTags) { AddIdentityGameplayTag(Tag); }
	}
}

/**
 * @brief Called when the container is replicated.
 * Compares the previous container with the current container to determine if any tags have been added or removed.
 *
 * @note Notifies observers locally.
 */
void UDuoIdentityComponent::OnRep_CurrentIdentityGameplayTags(FGameplayTagContainer PreviousIdentityGameplayTags)
{
	if (PreviousIdentityGameplayTags.Num() == CurrentIdentityGameplayTags.Num()) return;
	if (!Cast<APawn>(GetOwner())->IsLocallyControlled()) return;

	if (PreviousIdentityGameplayTags.Num() < CurrentIdentityGameplayTags.Num()) // Assume an Action got added.
	{
		for (const auto& Tag : CurrentIdentityGameplayTags)
		{
			if (!PreviousIdentityGameplayTags.HasTag(Tag)) { OnIdentityAdded.Broadcast(Tag); }
		}
	}

	if (PreviousIdentityGameplayTags.Num() > CurrentIdentityGameplayTags.Num()) // Assume an Action got removed.
	{
		for (const auto& Tag : PreviousIdentityGameplayTags)
		{
			if (!CurrentIdentityGameplayTags.HasTag(Tag)) { OnIdentityRemoved.Broadcast(Tag); }
		}
	}
	PreviousIdentityGameplayTags = CurrentIdentityGameplayTags;
}

void UDuoIdentityComponent::AddIdentityGameplayTag_Implementation(const FGameplayTag Tag)
{
	const FGameplayTagContainer TransientGameplayTagContainer = CurrentIdentityGameplayTags; // Create a separate tag for calling OnRep on server as OnRep does not get called automatically there.
	CurrentIdentityGameplayTags.AddTag(Tag);
	OnRep_CurrentIdentityGameplayTags(TransientGameplayTagContainer);
}

void UDuoIdentityComponent::RemoveIdentityGameplayTag_Implementation(const FGameplayTag Tag)
{
	const FGameplayTagContainer TransientGameplayTagContainer = CurrentIdentityGameplayTags; // Create a separate tag for calling OnRep on server as OnRep does not get called automatically there.
	if (CurrentIdentityGameplayTags.RemoveTag(Tag))
	{
		OnRep_CurrentIdentityGameplayTags(TransientGameplayTagContainer);
	}
}

bool UDuoIdentityComponent::HasIdentity(const FGameplayTag Tag) const
{
	return CurrentIdentityGameplayTags.HasTag(Tag);
}

void UDuoIdentityComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

#if !UE_BUILD_SHIPPING
	FString Msg;
	Msg.Append("Tags: ");

	for (const FGameplayTag Tag : CurrentIdentityGameplayTags) Msg.Append(FString::Printf(TEXT("[%s], "), *Tag.GetTagName().ToString()));

	if (Msg.Len() > 0) Msg.RemoveAt(Msg.Len() - 2, 2);

	LogOnScreen(this, Msg, FColor::White, 0.0F);
#endif
}

UDuoIdentityComponent* UDuoIdentityComponent::GetComponentFrom(AActor* FromActor) { return FromActor ? FromActor->FindComponentByClass<UDuoIdentityComponent>() : nullptr; }

void UDuoIdentityComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UDuoIdentityComponent, CurrentIdentityGameplayTags);
}
