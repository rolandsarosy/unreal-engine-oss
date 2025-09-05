#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Components/ActorComponent.h"
#include "DuoIdentityComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnIdentityAdded, FGameplayTag, IdentityAdded);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnIdentityRemoved, FGameplayTag, IdentityRemoved);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class DUOGAME_API UDuoIdentityComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UDuoIdentityComponent();

	UPROPERTY(ReplicatedUsing="OnRep_CurrentIdentityGameplayTags", VisibleAnywhere, BlueprintReadOnly, Category="Identities")
	FGameplayTagContainer CurrentIdentityGameplayTags;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Identities")
	FGameplayTagContainer DefaultIdentityGameplayTags;

	UPROPERTY(BlueprintAssignable, Category="Identities")
	FOnIdentityAdded OnIdentityAdded;

	UPROPERTY(BlueprintAssignable, Category="Identities")
	FOnIdentityRemoved OnIdentityRemoved;

	UFUNCTION(Server, Reliable, BlueprintCallable, Category="Identities", meta=(Tooltip = "Should be used for adding keycard-style Identity GameplayTags."))
	void AddIdentityGameplayTag(const FGameplayTag Tag);

	UFUNCTION(Server, Reliable, BlueprintCallable, Category="Identities", meta=(Tooltip = "Should be used for removing keycard-style Identity GameplayTags."))
	void RemoveIdentityGameplayTag(const FGameplayTag Tag);

	UFUNCTION(BlueprintCallable, Category="Identities", meta=(Tooltip = "Should be used for removing keycard-style Identity GameplayTags."))
	bool HasIdentity(const FGameplayTag Tag) const;

	UFUNCTION(BlueprintCallable, Category="Identities", meta=(DisplayName = "Get IdentityComponent From Actor", Tooltip = "Returns the IdentityComponent if the Actor has any. Otherwise a nullptr."))
	static UDuoIdentityComponent* GetComponentFrom(AActor* FromActor);

protected:
	UFUNCTION()
	void OnRep_CurrentIdentityGameplayTags(FGameplayTagContainer PreviousIdentityGameplayTags);

	virtual void BeginPlay() override;

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
};
