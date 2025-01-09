// Copyright Epic Games, Inc. All Rights Reserved.

#include "LanguageSwitcher.h"
#include "LanguageSwitcherStyle.h"
#include "LanguageSwitcherCommands.h"
#include "Misc/MessageDialog.h"
#include "ToolMenus.h"

#include "Internationalization/Culture.h"

static const FName LanguageSwitcherTabName("LanguageSwitcher");

#define LOCTEXT_NAMESPACE "FLanguageSwitcherModule"

void FLanguageSwitcherModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
	
	FLanguageSwitcherStyle::Initialize();
	FLanguageSwitcherStyle::ReloadTextures();

	FLanguageSwitcherCommands::Register();
	
	PluginCommands = MakeShareable(new FUICommandList);

	PluginCommands->MapAction(
		FLanguageSwitcherCommands::Get().PluginAction,
		FExecuteAction::CreateRaw(this, &FLanguageSwitcherModule::PluginButtonClicked),
		FCanExecuteAction());

	UToolMenus::RegisterStartupCallback(FSimpleMulticastDelegate::FDelegate::CreateRaw(this, &FLanguageSwitcherModule::RegisterMenus));
}

void FLanguageSwitcherModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.

	UToolMenus::UnRegisterStartupCallback(this);

	UToolMenus::UnregisterOwner(this);

	FLanguageSwitcherStyle::Shutdown();

	FLanguageSwitcherCommands::Unregister();
}

void FLanguageSwitcherModule::PluginButtonClicked()
{
	FInternationalization& I18N = FInternationalization::Get();
	FCulturePtr EditorLanguage = I18N.GetCurrentLanguage();
	UE_LOG(LogTemp, Warning, TEXT("Current Editor Language | GetDisplayName: %s"), *EditorLanguage->GetDisplayName());	// English
	UE_LOG(LogTemp, Warning, TEXT("Current Editor Language | GetEnglishName: %s"), *EditorLanguage->GetEnglishName());	// English
	UE_LOG(LogTemp, Warning, TEXT("Current Editor Language | GetNativeName: %s"), *EditorLanguage->GetNativeName());      // English 
	UE_LOG(LogTemp, Warning, TEXT("Current Editor Language | GetName: %s"), *EditorLanguage->GetName());                  // en

	if (EditorLanguage->GetName() == TEXT("en"))
	{
		I18N.SetCurrentLanguage(TEXT("zh-Hans"));
		// I18N.SetCurrentCulture(TEXT("zh-Hans"));
		I18N.SetCurrentLocale(TEXT("zh-Hans"));
	}
	else if (EditorLanguage->GetName() == TEXT("zh-Hans"))
	{
		I18N.SetCurrentLanguage(TEXT("en"));
		// I18N.SetCurrentCulture(TEXT("zh-Hans"));
		I18N.SetCurrentLocale(TEXT("en"));
	}
	// FCultureRef EditorCulture = I18N.GetCurrentCulture();
}

void FLanguageSwitcherModule::RegisterMenus()
{
	// Owner will be used for cleanup in call to UToolMenus::UnregisterOwner
	FToolMenuOwnerScoped OwnerScoped(this);

	{
		UToolMenu* Menu = UToolMenus::Get()->ExtendMenu("LevelEditor.MainMenu.Window");
		{
			FToolMenuSection& Section = Menu->FindOrAddSection("WindowLayout");
			Section.AddMenuEntryWithCommandList(FLanguageSwitcherCommands::Get().PluginAction, PluginCommands);
		}
	}

	{
		UToolMenu* ToolbarMenu = UToolMenus::Get()->ExtendMenu("LevelEditor.LevelEditorToolBar.PlayToolBar");
		{
			FToolMenuSection& Section = ToolbarMenu->FindOrAddSection("PluginTools");
			{
				FToolMenuEntry& Entry = Section.AddEntry(FToolMenuEntry::InitToolBarButton(FLanguageSwitcherCommands::Get().PluginAction));
				Entry.SetCommandList(PluginCommands);
			}
		}
	}
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FLanguageSwitcherModule, LanguageSwitcher)