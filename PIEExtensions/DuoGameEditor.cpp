#include "DuoGameEditor.h"

#include "LevelEditor.h"
#include "Styling/SlateStyleRegistry.h"

#define IMAGE_BRUSH(RelativePath, ...) FSlateImageBrush(StyleSet->RootToContentDir(RelativePath, TEXT(".png")), __VA_ARGS__)

// ReSharper disable once CppMemberFunctionMayBeStatic ~ incorrect suggestion
void DuoGameEditor::AddToolbarButtons(FToolBarBuilder& Builder) const
{
	Builder.AddSeparator(
		NAME_None,
		EVisibility::Visible
	);

	Builder.AddToolBarButton(
		FUIAction(
			FExecuteAction::CreateStatic(&DuoGameEditor::LaunchPlayInEditor, 1, PlayMode_InViewPort),
			FCanExecuteAction::CreateLambda([]() { return !GEditor->IsPlaySessionInProgress(); })
		),
		NAME_None,
		FText::FromName("PIE 1P"),
		FText::FromString("Launch a Play In Editor session with 1 Player in Selected Viewport."),
		FSlateIcon(StyleSet->GetStyleSetName(), "DuoGameEditor.PlayInViewport")
	);

	Builder.AddToolBarButton(
		FUIAction(
			FExecuteAction::CreateStatic(&DuoGameEditor::LaunchPlayInEditor, 2, PlayMode_InEditorFloating),
			FCanExecuteAction::CreateLambda([]() { return !GEditor->IsPlaySessionInProgress(); })
		), NAME_None,
		FText::FromName("PIE 2P"),
		FText::FromString("Launch a Play In Editor session with 2 Players in New Editor Windows."),
		FSlateIcon(StyleSet->GetStyleSetName(), "DuoGameEditor.PlayInEditorFloating")
	);
}

void DuoGameEditor::StartupModule()
{
	if (IsRunningCommandlet()) return;

	StyleSet = MakeShared<FSlateStyleSet>("DuoGameEditorStyle");
	const FString ContentDir = FPaths::ProjectDir() / TEXT("Source/DuoGameEditor/Resources/");
	StyleSet->SetContentRoot(ContentDir);

	const FVector2D Icon40X40(40.0f, 40.0f);

	StyleSet->Set("DuoGameEditor.PlayInViewport", new IMAGE_BRUSH("Icons/icon_playInViewport_40x", Icon40X40));
	StyleSet->Set("DuoGameEditor.PlayInEditorFloating", new IMAGE_BRUSH("Icons/icon_playInEditorFloating_40x", Icon40X40));

	FSlateStyleRegistry::RegisterSlateStyle(*StyleSet.Get());

	if (FModuleManager::Get().IsModuleLoaded("LevelEditor"))
	{
		FLevelEditorModule& LevelEditorModule = FModuleManager::LoadModuleChecked<FLevelEditorModule>("LevelEditor");
		const TSharedPtr<FExtender> ToolbarExtender = MakeShareable(new FExtender);
		ToolbarExtender->AddToolBarExtension("Play", EExtensionHook::After, nullptr, FToolBarExtensionDelegate::CreateRaw(this, &DuoGameEditor::AddToolbarButtons));
		LevelEditorModule.GetToolBarExtensibilityManager()->AddExtender(ToolbarExtender);
	}
}

void DuoGameEditor::ShutdownModule()
{
	if (StyleSet.IsValid())
	{
		FSlateStyleRegistry::UnRegisterSlateStyle(*StyleSet.Get());
		StyleSet.Reset();
	}
}

void DuoGameEditor::LaunchPlayInEditor(const int32 NumPlayers, const EPlayModeType PlayModeType)
{
	ULevelEditorPlaySettings* PlaySettings = GetMutableDefault<ULevelEditorPlaySettings>();
	PlaySettings->SetPlayNumberOfClients(NumPlayers);
	PlaySettings->SetPlayNetMode(PIE_ListenServer);
	PlaySettings->LastExecutedPlayModeType = PlayModeType;
	PlaySettings->PostEditChange();

	FRequestPlaySessionParams SessionParams;
	SessionParams.EditorPlaySettings = PlaySettings;
	SessionParams.WorldType = EPlaySessionWorldType::PlayInEditor;

	if (PlayModeType == PlayMode_InViewPort)
	{
		FLevelEditorModule& LevelEditorModule = FModuleManager::GetModuleChecked<FLevelEditorModule>("LevelEditor");

		if (const TSharedPtr<IAssetViewport> ActiveViewport = LevelEditorModule.GetFirstActiveViewport(); ActiveViewport.IsValid())
		{
			SessionParams.DestinationSlateViewport = ActiveViewport;
		}
	}

	GEditor->RequestPlaySession(SessionParams);
}

IMPLEMENT_MODULE(DuoGameEditor, DuoGameEditor)
