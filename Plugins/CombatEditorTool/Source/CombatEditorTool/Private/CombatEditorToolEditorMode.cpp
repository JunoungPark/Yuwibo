// Copyright Epic Games, Inc. All Rights Reserved.

#include "CombatEditorToolEditorMode.h"
#include "CombatEditorToolEditorModeToolkit.h"
#include "EdModeInteractiveToolsContext.h"
#include "InteractiveToolManager.h"
#include "CombatEditorToolEditorModeCommands.h"


//////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////// 
// AddYourTool Step 1 - include the header file for your Tools here
//////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////// 
#include "Tools/CombatEditorToolSimpleTool.h"
#include "Tools/CombatEditorToolInteractiveTool.h"

// step 2: register a ToolBuilder in FCombatEditorToolEditorMode::Enter() below


#define LOCTEXT_NAMESPACE "CombatEditorToolEditorMode"

const FEditorModeID UCombatEditorToolEditorMode::EM_CombatEditorToolEditorModeId = TEXT("EM_CombatEditorToolEditorMode");

FString UCombatEditorToolEditorMode::SimpleToolName = TEXT("CombatEditorTool_ActorInfoTool");
FString UCombatEditorToolEditorMode::InteractiveToolName = TEXT("CombatEditorTool_MeasureDistanceTool");


UCombatEditorToolEditorMode::UCombatEditorToolEditorMode()
{
	FModuleManager::Get().LoadModule("EditorStyle");

	// appearance and icon in the editing mode ribbon can be customized here
	Info = FEditorModeInfo(UCombatEditorToolEditorMode::EM_CombatEditorToolEditorModeId,
		LOCTEXT("ModeName", "CombatEditorTool"),
		FSlateIcon(),
		true);
}


UCombatEditorToolEditorMode::~UCombatEditorToolEditorMode()
{
}


void UCombatEditorToolEditorMode::ActorSelectionChangeNotify()
{
}

void UCombatEditorToolEditorMode::Enter()
{
	UEdMode::Enter();

	//////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////
	// AddYourTool Step 2 - register the ToolBuilders for your Tools here.
	// The string name you pass to the ToolManager is used to select/activate your ToolBuilder later.
	//////////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////// 
	const FCombatEditorToolEditorModeCommands& SampleToolCommands = FCombatEditorToolEditorModeCommands::Get();

	RegisterTool(SampleToolCommands.SimpleTool, SimpleToolName, NewObject<UCombatEditorToolSimpleToolBuilder>(this));
	RegisterTool(SampleToolCommands.InteractiveTool, InteractiveToolName, NewObject<UCombatEditorToolInteractiveToolBuilder>(this));

	// active tool type is not relevant here, we just set to default
	GetToolManager()->SelectActiveToolType(EToolSide::Left, SimpleToolName);
}

void UCombatEditorToolEditorMode::CreateToolkit()
{
	Toolkit = MakeShareable(new FCombatEditorToolEditorModeToolkit);
}

TMap<FName, TArray<TSharedPtr<FUICommandInfo>>> UCombatEditorToolEditorMode::GetModeCommands() const
{
	return FCombatEditorToolEditorModeCommands::Get().GetCommands();
}

#undef LOCTEXT_NAMESPACE
