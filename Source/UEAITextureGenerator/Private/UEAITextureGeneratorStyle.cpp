// Copyright Your Company. All Rights Reserved.

#include "UEAITextureGeneratorStyle.h"
#include "Styling/SlateStyleRegistry.h"
#include "Framework/Application/SlateApplication.h"
#include "Interfaces/IPluginManager.h"
#include "Styling/SlateStyleMacros.h"

#define RootToContentDir Style->RootToContentDir

TSharedPtr<FSlateStyleSet> FUEAITextureGeneratorStyle::StyleInstance = nullptr;

void FUEAITextureGeneratorStyle::Initialize()
{
	if (!StyleInstance.IsValid())
	{
		StyleInstance = Create();
		FSlateStyleRegistry::RegisterSlateStyle(*StyleInstance);
	}
}

void FUEAITextureGeneratorStyle::Shutdown()
{
	FSlateStyleRegistry::UnRegisterSlateStyle(*StyleInstance);
	ensure(StyleInstance.IsUnique());
	StyleInstance.Reset();
}

FName FUEAITextureGeneratorStyle::GetStyleSetName()
{
	static FName StyleSetName(TEXT("UEAITextureGeneratorStyle"));
	return StyleSetName;
}

void FUEAITextureGeneratorStyle::ReloadTextures()
{
	if (FSlateApplication::IsInitialized())
	{
		FSlateApplication::Get().GetRenderer()->ReloadTextureResources();
	}
}

const ISlateStyle& FUEAITextureGeneratorStyle::Get()
{
	return *StyleInstance;
}

const FVector2D Icon20x20(20.0f, 20.0f);

TSharedRef<FSlateStyleSet> FUEAITextureGeneratorStyle::Create()
{
	TSharedRef<FSlateStyleSet> Style = MakeShareable(new FSlateStyleSet("UEAITextureGeneratorStyle"));
	Style->SetContentRoot(IPluginManager::Get().FindPlugin("UEAITextureGenerator")->GetBaseDir() / TEXT("Resources"));

	// Icon for the plugin button
	Style->Set("UEAITextureGenerator.OpenPluginWindow", new IMAGE_BRUSH_SVG(TEXT("PlaceholderButtonIcon"), Icon20x20));

	return Style;
}

#undef IMAGE_BRUSH