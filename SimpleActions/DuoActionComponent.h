#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Components/ActorComponent.h"
#include "DuoActionComponent.generated.h"

class UDuoBaseAction;

/** 
 * @brief Parameterized Multicast Delegate that responds to a @UCBaseAction being added to an @UDuoActionComponent.

 * @param UDuoBaseAction: The action being removed from the component.
 *
 * Subscribed listeners can use this, in combination with the other delegates, to react to changes to the owner's abilities and effects.
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnActionAdded, UDuoBaseAction*, ActionAdded);

/** 
 * @brief Parameterized Multicast Delegate that responds to a @UDuoBaseAction being removed from an @UDuoActionComponent.
 *
 * @param UDuoBaseAction: The action being removed from the component.
 *
 * Subscribed listeners can use this, in combination with the other delegates, to react to changes to the owner's abilities and effects.
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnActionRemoved, UDuoBaseAction*, ActionRemoved);

/** 
 * @brief Parameterized Multicast Delegate that responds to an existing @UDuoBaseAction being started inside an @UDuoActionComponent
 *
 * @note That actions such as effects that are auto-started will trigger this delegate, right after being added to the @UDuoActionComponent.
 * 
 * @param UDuoBaseAction: The action being started inside a component.
 * @param AActor: The instigator, who is responsible for starting this action.
 *
 * Subscribed listeners can use this, in combination with the other delegates, to react to changes to the owner's abilities and effects.
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnActionStarted, UDuoBaseAction*, ActionStarted, AActor*, Instigator);

/** 
 * @brief Parameterized Multicast Delegate that responds to an existing @UDuoBaseAction being stopped inside an @UDuoActionComponent
 *
 * @param UDuoBaseAction: The action being stopped inside a component.
 * @param AActor: The instigator, who is responsible for stopping this action.
 *
 * Subscribed listeners can use this, in combination with the other delegates, to react to changes to the owner's abilities and effects.
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnActionStopped, UDuoBaseAction*, ActionStopped, AActor*, Instigator);

/**
 * @class UDuoActionComponent
 * @brief The class for managing GAS-like actions of an actor.
 *
 * The @UDuoActionComponent class provides functionality to add, start, and stop actions for an @AActor.
 * It manages a collection of @UDuoBaseACtion objects and provides convenient methods for interacting with them.
 */
UCLASS(ClassGroup=(Custom))
class DUOGAME_API UDuoActionComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UDuoActionComponent();
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Tags")
	FGameplayTagContainer ActiveGameplayTags;

	UPROPERTY(ReplicatedUsing="OnRep_CurrentActions", VisibleAnywhere, BlueprintReadOnly, Category="Actions")
	TArray<TObjectPtr<UDuoBaseAction>> CurrentActions;

	UPROPERTY(BlueprintAssignable, Category="Actions")
	FOnActionAdded OnActionAdded;

	UPROPERTY(BlueprintAssignable, Category="Actions")
	FOnActionRemoved OnActionRemoved;

	UPROPERTY(BlueprintAssignable, Category="Actions")
	FOnActionStarted OnActionStarted;

	UPROPERTY(BlueprintAssignable, Category="Actions")
	FOnActionStopped OnActionStopped;

	UFUNCTION(BlueprintCallable, Category="Actions")
	void AddAction(TSubclassOf<UDuoBaseAction> ActionClass, AActor* Instigator);

	UFUNCTION(BlueprintCallable, Category="Actions")
	void RemoveAction(UDuoBaseAction* ActionToRemove, AActor* Instigator);

	UFUNCTION(BlueprintCallable, Category="Actions")
	bool StartActionByTag(AActor* Instigator, FGameplayTag Tag);

	UFUNCTION(BlueprintCallable, Category="Actions")
	bool StopActionByTag(AActor* Instigator, FGameplayTag Tag);

	UFUNCTION(BlueprintCallable, Category="Actions", meta=(DisplayName = "Get ActionComponent From Actor", Tooltip = "Returns the ActionComponent if the Actor has any. Otherwise a nullptr."))
	static UDuoActionComponent* GetComponentFrom(AActor* FromActor);

private:
	UPROPERTY(EditAnywhere, Category="Actions")
	TArray<TSubclassOf<UDuoBaseAction>> DefaultActions;

	UFUNCTION(Server, Reliable)
	void ServerStartAction(AActor* Instigator, FGameplayTag Tag);

	UFUNCTION(Server, Reliable)
	void ServerStopAction(AActor* Instigator, FGameplayTag Tag);

	UFUNCTION()
	void OnRep_CurrentActions(TArray<UDuoBaseAction*> PreviousActions);

	virtual void BeginPlay() override;

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	virtual bool ReplicateSubobjects(UActorChannel* Channel, FOutBunch* Bunch, FReplicationFlags* RepFlags) override;
};
