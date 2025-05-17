// Copyright Your Company. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"
#include "Widgets/SWindow.h"
#include "Http.h"
#include "Widgets/Notifications/SProgressBar.h"

class SUEAITextureGeneratorWindow : public SWindow
{
public:
	virtual void Construct(const FArguments& InArgs);

private:
	// UI Elements
	TSharedPtr<SEditableTextBox> PromptTextBox;
	TSharedPtr<SComboBox<TSharedPtr<FString>>> SizeComboBox;
	TSharedPtr<SProgressBar> ProgressBar;
	TSharedPtr<STextBlock> StatusText;
	TSharedPtr<SButton> GenerateButton;
    
	// Size options
	TArray<TSharedPtr<FString>> SizeOptions;
	FString SelectedSize;
    
	// API Key
	FString ApiKey;
    
	// Event handlers
	FReply OnGenerateClicked();
	FReply OnCancelClicked();
	TSharedRef<SWidget> MakeSizeOptionWidget(TSharedPtr<FString> Option);
	void OnSizeSelectionChanged(TSharedPtr<FString> NewSelection, ESelectInfo::Type SelectInfo);
    
	// HTTP Request functions
	void SendOpenAIRequest(const FString& Prompt, const FString& TextureSize);
	void OnResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);
	
	// UI State
	void SetUIState(bool bEnabled, float ProgressValue, const FString& StatusMessage);
	void SetUIEnabled(bool bEnabled);
	void UpdateProgress(float ProgressValue);
	void UpdateStatus(const FString& StatusMessage);
    
	// HTTP Request handle (for cancellation)
	TSharedPtr<IHttpRequest, ESPMode::ThreadSafe> CurrentRequest;

	// Texture saving
	void SaveGeneratedImageAsTexture2D(const TArray<uint8>& ImageData);
};