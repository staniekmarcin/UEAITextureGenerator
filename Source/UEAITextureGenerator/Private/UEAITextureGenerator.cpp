// Copyright Your Company. All Rights Reserved.

#include "UEAITextureGenerator.h"
#include "UEAITextureGeneratorStyle.h"
#include "UEAITextureGeneratorCommands.h"
#include "Widgets/Docking/SDockTab.h"
#include "Widgets/Layout/SBox.h"
#include "Widgets/Text/STextBlock.h"
#include "ISettingsModule.h"
#include "ISettingsSection.h"
#include "ToolMenus.h"

#define LOCTEXT_NAMESPACE "FUEAITextureGeneratorModule"

void FUEAITextureGeneratorModule::StartupModule()
{
    // Initialize the style set
    FUEAITextureGeneratorStyle::Initialize();
    FUEAITextureGeneratorStyle::ReloadTextures();

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
    
    // Unregister commands and style
    FUEAITextureGeneratorStyle::Shutdown();
    FUEAITextureGeneratorCommands::Unregister();
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