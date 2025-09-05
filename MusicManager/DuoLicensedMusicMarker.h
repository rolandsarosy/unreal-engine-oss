#pragma once

#include "CoreMinimal.h"
#include "Engine/AssetUserData.h"
#include "DuoLicensedMusicMarker.generated.h"

/**
 * An empty UserAssetData class that is meant to be assigned to SoundWaves to denote that they are licensed, and thus should be excluded if the appropriate user settings are met.
 */
UCLASS(NotBlueprintable)
class DUOGAME_API UDuoLicensedMusicMarker : public UAssetUserData
{
	GENERATED_BODY()
};
