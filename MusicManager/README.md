# Replicated, synchronized music manager actor.
An in-world actor to act as a replicated, synchronized music player for multiplayer games. Supports licensed track skipping, looping at specific points after track end, custom fadeins, fadeouts, etc..

## Features
- **Replicated:** Designed for multiplayer from the ground up.
- **Synchronized:** Players shall always be at the exact same spot in the playlist.
- **FadeIn & FadeOut customization:** You are able to set custom values for fadeins and fadeouts between songs.
- **Licensed Tracks marking:** Assets can be marked as licensed and skipped as desired, should they match a desired user setting.
- **Loop-back:** Mark a spot in the playlist where the music starts to loop back, after the list ends.

## Design

![image]()

## Core components
- `MusicManager`: Actor that replicates and plays music in the world.
- `LicensedMusicMarker`: Add this marker to SoundWaves, SoundCues or MetaSounds at your discretion to mark them as licensed. Useful for 'Streamer Mode' for example. :)

## How to Use
- Add a single instance of `MusicManager` to your world and set music tracks and settings on it.
- Mark desired tracks with the `LicensedMusicMarker` class to be able to implement logic to skip them automatically.
- Call `ServerStartPlayback` to start the music player at any point in your gameplay.

## Example
Below is a common implementation for a door that requires a tag to unlock, and subsequently open:

![image]()

## Dependencies
- None.

## TODO
- I'd like to add the designed, automatic ducking feature when other voice channels trigger (such as voiceover), but that's not in yet.
- Because a 'Streamer Mode' or automated skipping is such a project-specific feature, you'll need to implement that on your own. Every project's needs would differ from a design standpoint on this issue. 
