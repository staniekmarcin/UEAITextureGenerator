// Copyright Your Company. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

class FToolBarBuilder;
class FMenuBuilder;

class FUEAITextureGeneratorModule : public IModuleInterface
{
public:
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
	void RegisterSettings();
	void UnregisterSettings();
	
private:
	void PluginButtonClicked();
	void CreateUI();
	void RegisterMenus();
	
	TSharedPtr<class FUICommandList> PluginCommands;
	TSharedPtr<class SUEAITextureGeneratorWindow> TextureGeneratorWindow;
};