// Copyright Epic Games, Inc. All Rights Reserved.

#include "LanguageSwitcherCommands.h"

#define LOCTEXT_NAMESPACE "FLanguageSwitcherModule"

void FLanguageSwitcherCommands::RegisterCommands()
{
	UI_COMMAND(PluginAction, "LanguageSwitcher", "Change Engine Language to English/Chinese", EUserInterfaceActionType::Button, FInputChord());
}

#undef LOCTEXT_NAMESPACE
