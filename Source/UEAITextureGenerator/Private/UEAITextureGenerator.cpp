// Copyright Your Company. All Rights Reserved.

#include "UEAITextureGenerator.h"
#include "UEAITextureGeneratorStyle.h"
#include "UEAITextureGeneratorCommands.h"
#include "Widgets/Docking/SDockTab.h"
#include "Widgets/Layout/SBox.h"
#include "Widgets/Text/STextBlock.h"
#include "ISettingsModule.h"
#include "ToolMenus.h"
#include "UEAITextureGeneratorSettings.h"

#define LOCTEXT_NAMESPACE "FUEAITextureGeneratorModule"

void FUEAITextureGeneratorModule::StartupModule()
{
    // Initialize the style set
    FUEAITextureGeneratorStyle::Initialize();
    FUEAITextureGeneratorStyle::ReloadTextures();

    // Register settings
    RegisterSettings();
    
    // Initialize commands
    FUEAITextureGeneratorCommands::Register();
    
    // Mapping commands
    PluginCommands = MakeShareable(new FUICommandList);
    PluginCommands->MapAction(
        FUEAITextureGeneratorCommands::Get().OpenPluginWindow,
        FExecuteAction::CreateRaw(this, &FUEAITextureGeneratorModule::PluginButtonClicked),
        FCanExecuteAction());

    // Register menus
    RegisterMenus();
}

void FUEAITextureGeneratorModule::ShutdownModule()
{
    // Unregister menus
    UToolMenus::UnRegisterStartupCallback(this);
    UToolMenus::UnregisterOwner(this);

    // Unregister settings
    UnregisterSettings();
    
    // Unregister commands and style
    FUEAITextureGeneratorStyle::Shutdown();
    FUEAITextureGeneratorCommands::Unregister();
}

void FUEAITextureGeneratorModule::RegisterSettings()
{
    if (ISettingsModule* SettingsModule = FModuleManager::GetModulePtr<ISettingsModule>("Settings"))
    {
        SettingsModule->RegisterSettings(
            "Project", 
            "Plugins", 
            "UEAI Texture Generator",
            LOCTEXT("UEAISettingsName", "UEAI Texture Generator"),
            LOCTEXT("UEAISettingsDescription", "Configure the OpenAI API key and other options."),
            GetMutableDefault<UUEAITextureGeneratorSettings>()
        );
    }
}

void FUEAITextureGeneratorModule::UnregisterSettings()
{
    if (ISettingsModule* SettingsModule = FModuleManager::GetModulePtr<ISettingsModule>("Settings"))
    {
        SettingsModule->UnregisterSettings("Project", "Plugins", "UEAI Texture Generator");
    }
}

void FUEAITextureGeneratorModule::PluginButtonClicked()
{
    
}

void FUEAITextureGeneratorModule::RegisterMenus()
{
    // Owner will be used for cleanup in call to UnregisterOwner
    FToolMenuOwnerScoped OwnerScoped(this);

    // Add menu extension to the tools menu
    UToolMenu* Menu = UToolMenus::Get()->ExtendMenu("LevelEditor.MainMenu.Tools");
    FToolMenuSection& Section = Menu->FindOrAddSection("Tools");
    Section.AddMenuEntryWithCommandList(FUEAITextureGeneratorCommands::Get().OpenPluginWindow, PluginCommands);
}

#undef LOCTEXT_NAMESPACE
    
IMPLEMENT_MODULE(FUEAITextureGeneratorModule, UEAITextureGenerator)