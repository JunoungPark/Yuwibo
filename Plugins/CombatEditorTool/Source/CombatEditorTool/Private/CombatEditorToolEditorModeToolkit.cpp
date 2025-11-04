// Copyright Epic Games, Inc. All Rights Reserved.

#include "CombatEditorToolEditorModeToolkit.h"
#include "CombatEditorToolEditorMode.h"
#include "Engine/Selection.h"

#include "Modules/ModuleManager.h"
#include "PropertyEditorModule.h"
#include "IDetailsView.h"
#include "EditorModeManager.h"

#define LOCTEXT_NAMESPACE "CombatEditorToolEditorModeToolkit"

FCombatEditorToolEditorModeToolkit::FCombatEditorToolEditorModeToolkit()
{
}

void FCombatEditorToolEditorModeToolkit::Init(const TSharedPtr<IToolkitHost>& InitToolkitHost, TWeakObjectPtr<UEdMode> InOwningMode)
{
	FModeToolkit::Init(InitToolkitHost, InOwningMode);
}

void FCombatEditorToolEditorModeToolkit::GetToolPaletteNames(TArray<FName>& PaletteNames) const
{
	PaletteNames.Add(NAME_Default);
}


FName FCombatEditorToolEditorModeToolkit::GetToolkitFName() const
{
	return FName("CombatEditorToolEditorMode");
}

FText FCombatEditorToolEditorModeToolkit::GetBaseToolkitName() const
{
	return LOCTEXT("DisplayName", "CombatEditorToolEditorMode Toolkit");
}

#undef LOCTEXT_NAMESPACE
