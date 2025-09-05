# Editor extension that adds custom PlayInEditor buttons with specific behavior
An editor-only module for those working on co-op games. Creates additional buttons which start the PIE session with different window configs, player counts, netmodes. All my homies hate dropdown menus.

## Behavior
As a developer who works with 2-player co-op games, I _really_ often needed to switch between in-editor 1-player session and new-window 2-player sessions. It takes a second or two to change that. With this extension, it does not.

You will likely need to modify this to your projects needs but for now, the two created buttons do the following:
- `"Launch a Play In Editor session with 1 Player in Selected Viewport."`
- `"Launch a Play In Editor session with 2 Players in New Editor Windows."`

## How to Use
- Make a new module in your project.
- Make sure to set the module as `Type = TargetType.Editor` so that it does not get compiled into builds.
- Add the relevant code (and likely modify it to your needs) to the module.
- Recompile & enjoy.

## Example:

![image](https://github.com/rolandsarosy/unreal-engine-oss/blob/main/PIEExtensions/img_editor_ext_buttons.png?raw=true)

## Dependencies
- None.
