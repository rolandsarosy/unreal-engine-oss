#pragma once

#include "CoreMinimal.h"
#include "DuoGame/Public/GameplayAbilities/DuoBaseAction.h"
#include "DuoActionAbility.generated.h"

/**
 * @class UDuoActionAbility
 *
 * @brief Represents an ability that an actor can perform.
 *
 * The UDuoActionAbility class is a subclass of UDuoBaseAction. Almost the same as the UDuoBaseAction with the exception that it's been explicitly specified that it doesn't auto start.
 */
UCLASS(Blueprintable, Abstract)
class DUOGAME_API UDuoActionAbility : public UDuoBaseAction
{
	GENERATED_BODY()

public:
	UDuoActionAbility();
};
