# Gameplay Identity
A simple, replicated, Doom-inspired keycard system backed by Gameplay Tags. Designed for creating locks, access levels, or any identity-based gameplay mechanic for actors like doors, chests, and terminals.

## Features
- **Replicated:** Designed for multiplayer from the ground up.
- **Gameplay Tag Driven:** Define any number of "keycards" or "identities" with Unreal's native Gameplay Tag system.
- **Event-Driven:** Components can listen for when specific identities are added or removed.
- **Component-Based:** Easy to add to any pawn or character.

## Core components
- `IdentityComponent`: Add this to any actor that needs to possess identities (e.g., the player character). It manages a list of granted Gameplay Tags.

## How to Use
- Add the `IdentityComponent` to your Character/Pawn. This will allow the character to hold identities.
- Define GameplayTags in Project Settings. Navigate to `Edit` > `Project Settings` > `Gameplay Tags` and create your identity tags (e.g., `Identity.Key.Red`, `Identity.Access.Admin`, `Identity.State.Corrupted`). Alternatively, I highly recommend declaring your GameplayTags in C++ so you are able to use them in both Blueprints and code.
- Grant and Revoke Identities. Call the `AddIdentityGameplayTag` and `RemoveIdentityGameplayTag` functions on the `IdentityComponent` to give or take keys from a character. This can be done when a player picks up an item, completes a quest, etc. You are also able to set a list of `DefaultIdentityGameplayTags`. 
- Check for an existing identities. Read from `CurrentIdentityGameplayTags`, call `HasIdentity` or listen to changes with `FOnIdentityAdded` or `FOnIdentityRemoved`.

## Example
Below is a common implementation for a door that requires a tag to unlock, and subsequently open:

![image](https://github.com/rolandsarosy/unreal-engine-oss/blob/main/GameplayIdentity/img_identity_unlock_example.png)

## Dependencies
- The `GameplayTags` module must be enabled in your project.
