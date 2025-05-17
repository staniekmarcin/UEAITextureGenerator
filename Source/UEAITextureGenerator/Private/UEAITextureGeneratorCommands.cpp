// Copyright Your Company. All Rights Reserved.

#include "UEAITextureGeneratorCommands.h"

#define LOCTEXT_NAMESPACE "FUEAITextureGeneratorModule"

void FUEAITextureGeneratorCommands::RegisterCommands()
{
    UI_COMMAND(OpenPluginWindow, "Generate AI Texture", "Generate textures using OpenAI API", EUserInterfaceActionType::Button, FInputChord());
}

#undef LOCTEXT_NAMESPACE