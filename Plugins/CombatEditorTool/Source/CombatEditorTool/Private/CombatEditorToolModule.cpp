// Copyright Epic Games, Inc. All Rights Reserved.

#include "CombatEditorToolModule.h"
#include "CombatEditorToolEditorModeCommands.h"
#include "CombatWaveEditorController.h"
#include "Widget/SCombatWaveEditor.h"
#include "LevelEditor.h"
#include "Widgets/Docking/SDockTab.h"

#define LOCTEXT_NAMESPACE "CombatEditorToolModule"

void FCombatEditorToolModule::StartupModule()
{
    // 탭 등록
    FGlobalTabmanager::Get()->RegisterNomadTabSpawner(
        "CombatWaveEditor",
        FOnSpawnTab::CreateRaw(this, &FCombatEditorToolModule::OnSpawnPluginTab)
    )
        .SetDisplayName(LOCTEXT("CombatWaveEditorTabTitle", "Combat Wave Editor"))
        .SetMenuType(ETabSpawnerMenuType::Hidden);

    // 메뉴(윈도우 메뉴에 추가) - 간단한 예시
    if (UToolMenus::IsToolMenuUIEnabled())
    {
        UToolMenu* Menu = UToolMenus::Get()->ExtendMenu("LevelEditor.MainMenu.Window");
        FToolMenuSection& Section = Menu->FindOrAddSection("WindowLayout");
        Section.AddMenuEntry(
            "OpenCombatWaveEditor",
            LOCTEXT("OpenCombatWaveEditorLabel", "Combat Wave Editor"),
            LOCTEXT("OpenCombatWaveEditorTooltip", "Open the Combat Wave Editor tab"),
            FSlateIcon(),
            FUIAction(FExecuteAction::CreateLambda([]() {
                FGlobalTabmanager::Get()->TryInvokeTab(FTabId("CombatWaveEditor"));
                }))
        );
    }
}

void FCombatEditorToolModule::ShutdownModule()
{
    FGlobalTabmanager::Get()->UnregisterNomadTabSpawner("CombatWaveEditor");
}

TSharedRef<SDockTab> FCombatEditorToolModule::OnSpawnPluginTab(const FSpawnTabArgs& SpawnTabArgs)
{
    // 컨트롤러 생성 후 Slate 위젯에 전달
    TSharedPtr<FCombatWaveEditorController> Controller = MakeShared<FCombatWaveEditorController>();
    Controller->Initialize();

    return SNew(SDockTab)
        .TabRole(ETabRole::NomadTab)
        [
            SNew(SCombatWaveEditor)
                .Controller(Controller)
        ];
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FCombatEditorToolModule, CombatEditorToolEditorMode)