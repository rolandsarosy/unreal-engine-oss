#pragma once

class DuoGameEditor : public IModuleInterface
{
public:
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;

private:
	static void LaunchPlayInEditor(int32 NumPlayers, EPlayModeType PlayModeType);

	void AddToolbarButtons(FToolBarBuilder& Builder) const;
	
	TSharedPtr<FSlateStyleSet> StyleSet;
};
