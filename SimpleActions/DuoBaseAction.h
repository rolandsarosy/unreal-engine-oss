#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "DuoBaseAction.generated.h"

class UDuoActionComponent;

USTRUCT(BlueprintType)
struct FActionReplicationData
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly)
	bool bIsRunning;

	UPROPERTY(BlueprintReadOnly)
	AActor* Instigator;

	FActionReplicationData(): bIsRunning(false), Instigator(nullptr)
	{
	}
};

/**
 * @class UDuoBaseAction
 * @brief The base class for all GAS-like actions in the game. Direct children should be used as base classes for actions and effects.
 *
 * This class provides a common interface and functionality for starting and stopping actions.
 * Each action has an ActionName property that can be used to start and stop actions without a reference to the object.
 *
 * @note All Actions should be placed inside the @AActor's @CActionComponent. 
 */
UCLASS(Blueprintable, Abstract)
class DUOGAME_API UDuoBaseAction : public UObject
{
	GENERATED_BODY()

public:
	/** Action tag to start/stop without a reference to the object. **/
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Action")
	FGameplayTag Tag;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Action")
	FText ActionName;

	/** Start immediately when added to an action component. **/
	UPROPERTY(EditDefaultsOnly, Category="Action")
	bool bAutoStart;

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing="OnRep_ReplicationData")
	FActionReplicationData ReplicationData;

	UFUNCTION(BlueprintNativeEvent, Category="Action")
	void StartAction(AActor* Instigator);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="Action")
	void StopAction(AActor* Instigator);

	UFUNCTION(BlueprintNativeEvent, Category="Action")
	bool CanStart(AActor* Instigator);

	UFUNCTION(BlueprintCallable, Category="Action")
	bool IsRunning() const;

	virtual UWorld* GetWorld() const override;

protected:
	/** Tags added to an owning actor when activated, removed when actions stops. **/
	UPROPERTY(EditDefaultsOnly, Category="Tags")
	FGameplayTagContainer GrantsTags;

	/** Action can only start if OwningActor has none of these tags applied. **/
	UPROPERTY(EditDefaultsOnly, Category="Tags")
	FGameplayTagContainer BlockedTags;

	UPROPERTY(Replicated, BlueprintReadOnly, Category="Action")
	float TimeStarted;

	UFUNCTION(BlueprintCallable, Category="Action")
	UDuoActionComponent* GetOwningComponent() const;

private:
	UFUNCTION()
	void OnRep_ReplicationData();

	virtual bool IsSupportedForNetworking() const override { return true; }
};
