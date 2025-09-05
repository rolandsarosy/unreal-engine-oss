#include "Actors/DuoMusicManager.h"

#include "DuoLicensedMusicMarker.h"
#include "Components/AudioComponent.h"
#include "Components/BillboardComponent.h"
#include "DuoGame/DuoGame.h"

ADuoMusicManager::ADuoMusicManager()
{
	AudioComponent = CreateDefaultSubobject<UAudioComponent>("AudioComponent");
	AudioComponent->bAutoActivate = false;

	BillboardComponent = CreateDefaultSubobject<UBillboardComponent>("BillboardComponent");
	RootComponent = BillboardComponent;

	CurrentTrackIndex = 0;

	PrimaryActorTick.bCanEverTick = false;
	bReplicates = true;
}

void ADuoMusicManager::BeginPlay()
{
	Super::BeginPlay();
}

void ADuoMusicManager::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	GetWorld()->GetTimerManager().ClearTimer(FadeOutTimerHandle);
	GetWorld()->GetTimerManager().ClearTimer(OnTrackFinishedTimerHandle);

	Super::EndPlay(EndPlayReason);
}

void ADuoMusicManager::ServerStartPlayback_Implementation()
{
	UE_LOG(LogDuoGame, Verbose, TEXT("Music manager was requested to start the playback."))

	check(Tracks.Num() > 0 && "There are no tracks available in the music manager.");
	check(TrackLoopStartIndex < Tracks.Num() && "TrackLoopStartIndex is greater than the number of available tracks in the manager.");

	CurrentTrackIndex = 0;
	ServerPlayTrackAtIndex(CurrentTrackIndex);
}

void ADuoMusicManager::ServerPlayTrackAtIndex_Implementation(const uint8 Index)
{
	UE_LOG(LogDuoGame, VeryVerbose, TEXT("Music manager was requested to play track #%d,'%s'"), Index, *Tracks[CurrentTrackIndex].MusicTrack->GetName())

	GetWorld()->GetTimerManager().ClearTimer(FadeOutTimerHandle);
	GetWorld()->GetTimerManager().ClearTimer(OnTrackFinishedTimerHandle);

	const FDuoMusicTrackInfo CurrentTrackInfo = Tracks[Index];
	MulticastPlayTrack(CurrentTrackInfo.MusicTrack, CurrentTrackInfo.FadeInDuration);

	const float FadeOutTriggerTime = CurrentTrackInfo.MusicTrack->GetDuration() - CurrentTrackInfo.FadeOutDuration;
	const float NextTrackTime = CurrentTrackInfo.MusicTrack->GetDuration() + CurrentTrackInfo.PostTrackPauseDuration;

	GetWorld()->GetTimerManager().SetTimer(FadeOutTimerHandle, this, &ADuoMusicManager::ServerTriggerFadeOut, FadeOutTriggerTime, false);
	GetWorld()->GetTimerManager().SetTimer(OnTrackFinishedTimerHandle, this, &ADuoMusicManager::ServerOnTrackFinished, NextTrackTime, false);
}

void ADuoMusicManager::MulticastPlayTrack_Implementation(USoundBase* Track, float FadeInDuration)
{
	if (AudioComponent->IsPlaying())
	{
		// TODO: This should never occur but I think this FadeOut will be instantly overriden by the SetSound and FadeIn, so it's effectively ineffective.
		UE_LOG(LogDuoGame, Warning, TEXT("Music manager is currently playing a track. Will fade out the current track and switch to new track."))
		AudioComponent->FadeOut(1.0f, 0.0, EAudioFaderCurve::Linear);
	}

	if (Track->AssetUserData.Num() > 0 && Track->GetAssetUserDataOfClass(UDuoLicensedMusicMarker::StaticClass()))
	{
		// TODO: Add functionality here to skip licensed music if the appropriate user settings are found. 
		UE_LOG(LogDuoGame, Warning, TEXT("Music manager is playing a track that is licensed. No stub implementation is provided yet, this is merely a warning."))
	}

	AudioComponent->SetSound(Track);
	AudioComponent->FadeIn(FadeInDuration, 1.0, 0, EAudioFaderCurve::Linear);
}

void ADuoMusicManager::ServerTriggerFadeOut_Implementation()
{
	UE_LOG(LogDuoGame, VeryVerbose, TEXT("Music manager was requested to trigger a fade out for the track #%d,'%s'"), CurrentTrackIndex, *Tracks[CurrentTrackIndex].MusicTrack->GetName())

	MulticastTriggerFadeOut(Tracks[CurrentTrackIndex].FadeOutDuration);
}

void ADuoMusicManager::MulticastTriggerFadeOut_Implementation(const float FadeOutDuration)
{
	AudioComponent->FadeOut(FadeOutDuration, 0.0, EAudioFaderCurve::Linear);
}

void ADuoMusicManager::ServerOnTrackFinished_Implementation()
{
	UE_LOG(LogDuoGame, VeryVerbose, TEXT("Music manager calculates that a track is finished (including post-pause): #%d,'%s'"), CurrentTrackIndex, *Tracks[CurrentTrackIndex].MusicTrack->GetName())

	CurrentTrackIndex++;
	if (CurrentTrackIndex >= Tracks.Num()) CurrentTrackIndex = TrackLoopStartIndex;

	ServerPlayTrackAtIndex(CurrentTrackIndex);
}
