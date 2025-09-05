# GAS, but simplified.
A simplified, replicated GAS-like ability system. Incredibly lightweight and backed by gameplay tags. Supports Actions (one-shots) and effects (over-time) as well.

## Features
- **Replicated:** Designed for multiplayer from the ground up.
- **Server-authoritative:** The host/server is the single source of truth.
- **Simple:** GAS was a bit daunting for me to use in my tiny project. This is a much smaller scope.
- **Actions and effects:** Supports both one-time actions and over-time effects.
- **Backed by GameplayTags:** The entire feature set is backed and supported by GameplayTags, making it easy to use in most places in the engine.
- **Blueprint Support:** Designed to be used within Blueprints. 

## Core components
- `ActionComponent`: Added to Pawns/Characters to add the ability to hold and use actions.
- `BaseAction`: Base class for actions. Should not be used on its own.
- `ActionAbility`: Class for one-time actions such as jumping, using a flashlight, and so on.
- `ActionEffect`: Class for over-time actions such as a fire over-time effect after being burned, healing after picking up a medkit or maybe preventing the player from opening doors for a while.

## How to Use
- Add a single instance of `ActionComponent` to your desired Pawn, Character or AICharacter.
- Create a set of Actions (one-time or over-time). Think of things like, jumping, swimming, fireball, opening doors, interacting, etc.
    - Assign an Action's `GrantsTags` and `BlocksTags` as well as `ActionTag` fields with relevant information.
    - Overwrite the Action's `StartAction` and `StopAction` functions as desired.
    - If the action should not be able to be used at any point (granted no `BlockingTags`are found), you can overwrite the `CanStart` function to create logic for such cases.
- Assign Actions by default using the `ActionComponent`'s `DefaultActions` field or during gameplay with `AddAction` or `RemoveAction`
- You are able to manually trigger actions as well, with `StartAction` or` StopAction`, if the given action is not in a character's control.

## Example
Quick example I made to showcase how the action of jumping can be created. Perhaps sometimes during the level you choose to enable or disable this.

- In my Character.cpp


- In Character Blueprint


- Jump Action's Blueprint


## Dependencies
- `GameplayTags` must be enabled for the project.

## Note
Originally I made this on my own during Tom Looman's Unreal Engine C++ classes, but I've added so many changes and refactors that it no longer resembles the study material at all.
