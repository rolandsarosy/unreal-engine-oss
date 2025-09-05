#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/BillboardComponent.h"
#include "DuoMusicManager.generated.h"

USTRUCT(BlueprintType)
struct FDuoMusicTrackInfo
{
	GENERATED_BODY()

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Duo Music Manager")
	TObjectPtr<USoundBase> MusicTrack;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Duo Music Manager", meta = (ClampMin = "0", ClampMax = "15", Delta = "1"))
	float FadeInDuration;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Duo Music Manager", meta = (ClampMin = "0", ClampMax = "15", Delta = "1"))
	float FadeOutDuration;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Duo Music Manager", meta = (ClampMin = "0", ClampMax = "15", Delta = "1"))
	float PostTrackPauseDuration;

	FDuoMusicTrackInfo() : FadeInDuration(0.0f), FadeOutDuration(0.0f), PostTrackPauseDuration(0.0f)
	{
	}
};

UCLASS(Blueprintable)
class DUOGAME_API ADuoMusicManager : public AActor
{
	GENERATED_BODY()

public:
	ADuoMusicManager();

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category="Duo Music Manager")
	TArray<FDuoMusicTrackInfo> Tracks;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category="Duo Music Manager")
	uint8 TrackLoopStartIndex;

	UFUNCTION(Server, Reliable, BlueprintCallable, Category = "Duo Music Manager")
	void ServerStartPlayback();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Duo Music Manager")
	TObjectPtr<UAudioComponent> AudioComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Duo Music Manager")
	TObjectPtr<UBillboardComponent> BillboardComponent;

	FTimerHandle FadeOutTimerHandle;

	FTimerHandle OnTrackFinishedTimerHandle;

protected:
	UPROPERTY()
	uint8 CurrentTrackIndex;

	UFUNCTION(Server, Reliable)
	void ServerPlayTrackAtIndex(uint8 Index);

	UFUNCTION(Server, Reliable)
	void ServerTriggerFadeOut();

	UFUNCTION(Server, Reliable)
	void ServerOnTrackFinished();

	UFUNCTION(NetMulticast, Reliable)
	void MulticastPlayTrack(USoundBase* Track, float FadeInDuration);

	UFUNCTION(NetMulticast, Reliable)
	void MulticastTriggerFadeOut(float FadeOutDuration);

private:
	virtual void BeginPlay() override;

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
};
