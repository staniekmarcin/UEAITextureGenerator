// Copyright Your Company. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Framework/Commands/Commands.h"
#include "UEAITextureGeneratorStyle.h"

class FUEAITextureGeneratorCommands : public TCommands<FUEAITextureGeneratorCommands>
{
public:
	FUEAITextureGeneratorCommands()
		: TCommands<FUEAITextureGeneratorCommands>(
			FUEAITextureGeneratorStyle::GetStyleSetName(), 
			FText::FromString("UE AI Texture Generator"), 
			NAME_None, 
			FUEAITextureGeneratorStyle::GetStyleSetName()
		)
	{
	}

	// TCommands<> interface
	virtual void RegisterCommands() override;

public:
	TSharedPtr<FUICommandInfo> OpenPluginWindow;
};