using UnrealBuildTool;

public class DuoGameEditor : ModuleRules
{
	public DuoGameEditor(ReadOnlyTargetRules Target) : base(Target)
	{
		PublicDependencyModuleNames.AddRange([
			"Core",
			"CoreUObject",
			"Engine",
			"InputCore",
			"UnrealEd",
			"LevelEditor",
			"EditorStyle",
			"EditorSubsystem",
			"EditorWidgets",
			"Slate",
			"SlateCore",
		]);
	}
}