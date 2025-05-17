// Copyright Your Company. All Rights Reserved.

#include "UEAITextureGeneratorWindow.h"
#include "SlateOptMacros.h"
#include "Widgets/Layout/SBox.h"
#include "Widgets/Layout/SBorder.h"
#include "Widgets/Input/SButton.h"
#include "Widgets/Input/SEditableTextBox.h"
#include "Widgets/Input/SComboBox.h"
#include "Widgets/Text/STextBlock.h"
#include "Widgets/Layout/SScrollBox.h"
#include "Styling/SlateTypes.h"
#include "Modules/ModuleManager.h"
#include "Interfaces/IHttpResponse.h"
#include "Engine/Texture2D.h"
#include "Dom/JsonObject.h"
#include "Serialization/JsonReader.h"
#include "Serialization/JsonSerializer.h"
#include "UEAITextureGeneratorSettings.h"
#include "AssetRegistry/AssetRegistryModule.h"
#include "Factories/TextureFactory.h"

#define LOCTEXT_NAMESPACE "UEAITextureGenerator"

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION

void SUEAITextureGeneratorWindow::Construct(const FArguments& InArgs)
{
    SWindow::Construct(InArgs);
    
    // Initialize size options
    SizeOptions.Add(MakeShareable(new FString("1024x1024")));
    SizeOptions.Add(MakeShareable(new FString("1792x1024")));
    SelectedSize = *SizeOptions[0].Get();
    
    // Load API key from config or project settings
    ApiKey = GetDefault<UUEAITextureGeneratorSettings>()->OpenAIAPIKey;
    
    SetContent(
        SNew(SBorder)
        .BorderImage(FCoreStyle::Get().GetBrush("ToolPanel.GroupBorder"))
        .Padding(FMargin(8.0f))
        [
            SNew(SVerticalBox)
            
            // Title
            + SVerticalBox::Slot()
            .AutoHeight()
            .Padding(0, 0, 0, 10)
            [
                SNew(STextBlock)
                .Text(LOCTEXT("WindowTitle", "AI Texture Generator"))
                .Font(FCoreStyle::Get().GetFontStyle("HeadingFont"))
            ]
            
            // Prompt input
            + SVerticalBox::Slot()
            .AutoHeight()
            .Padding(0, 0, 0, 10)
            [
                SNew(SVerticalBox)
                + SVerticalBox::Slot()
                .AutoHeight()
                .Padding(0, 0, 0, 5)
                [
                    SNew(STextBlock)
                    .Text(LOCTEXT("PromptLabel", "Prompt:"))
                ]
                + SVerticalBox::Slot()
                .AutoHeight()
                [
                    SAssignNew(PromptTextBox, SEditableTextBox)
                    .MinDesiredWidth(300)
                    .HintText(LOCTEXT("PromptHint", "Enter a description of the texture you want to generate..."))
                ]
            ]
            
            // Size selection
            + SVerticalBox::Slot()
            .AutoHeight()
            .Padding(0, 0, 0, 10)
            [
                SNew(SVerticalBox)
                + SVerticalBox::Slot()
                .AutoHeight()
                .Padding(0, 0, 0, 5)
                [
                    SNew(STextBlock)
                    .Text(LOCTEXT("SizeLabel", "Texture Size:"))
                ]
                + SVerticalBox::Slot()
                .AutoHeight()
                [
                    SAssignNew(SizeComboBox, SComboBox<TSharedPtr<FString>>)
                    .OptionsSource(&SizeOptions)
                    .OnGenerateWidget(this, &SUEAITextureGeneratorWindow::MakeSizeOptionWidget)
                    .OnSelectionChanged(this, &SUEAITextureGeneratorWindow::OnSizeSelectionChanged)
                    .InitiallySelectedItem(SizeOptions[0])
                    [
                        SNew(STextBlock)
                        .Text_Lambda([this]() { return FText::FromString(SelectedSize); })
                    ]
                ]
            ]
            
            // Progress bar
            + SVerticalBox::Slot()
            .AutoHeight()
            .Padding(0, 0, 0, 10)
            [
                SNew(SVerticalBox)
                + SVerticalBox::Slot()
                .AutoHeight()
                .Padding(0, 0, 0, 5)
                [
                    SAssignNew(StatusText, STextBlock)
                    .Text(LOCTEXT("ReadyStatus", "Ready to generate"))
                ]
                + SVerticalBox::Slot()
                .AutoHeight()
                [
                    SAssignNew(ProgressBar, SProgressBar)
                    .Percent(0.0f)
                    .FillColorAndOpacity(FLinearColor(0.46f, 0.76f, 0.0f))
                ]
            ]
            
            // Buttons
            + SVerticalBox::Slot()
            .AutoHeight()
            .Padding(0, 10, 0, 0)
            [
                SNew(SHorizontalBox)
                + SHorizontalBox::Slot()
                .FillWidth(1.0f)
                [
                    SAssignNew(GenerateButton ,SButton)
                    .HAlign(HAlign_Center)
                    .VAlign(VAlign_Center)
                    .Text(LOCTEXT("GenerateButton", "Generate"))
                    .OnClicked(this, &SUEAITextureGeneratorWindow::OnGenerateClicked)
                ]
                + SHorizontalBox::Slot()
                .AutoWidth()
                .Padding(10, 0, 0, 0)
                [
                    SNew(SButton)
                    .HAlign(HAlign_Center)
                    .VAlign(VAlign_Center)
                    .Text(LOCTEXT("CancelButton", "Cancel"))
                    .OnClicked(this, &SUEAITextureGeneratorWindow::OnCancelClicked)
                ]
            ]
        ]
    );
}

FReply SUEAITextureGeneratorWindow::OnGenerateClicked()
{
    FString Prompt = PromptTextBox->GetText().ToString();
    if (Prompt.IsEmpty())
    {
        SetUIState(true, 0.0f, TEXT("Prompt cannot be empty"));
        return FReply::Handled();
    }
    
    // Send request to OpenAI
    SendOpenAIRequest(Prompt, SelectedSize);
    SetUIState(false, 0.1f, TEXT("Generating texture..."));
    return FReply::Handled();
}

FReply SUEAITextureGeneratorWindow::OnCancelClicked()
{
    // Cancel current request if any
    if (CurrentRequest.IsValid())
    {
        CurrentRequest->CancelRequest();
        CurrentRequest.Reset();
        SetUIState(true, 0.0f, TEXT("Request cancelled"));
    }
    else
    {
        // Close window
        RequestDestroyWindow();
    }
    
    return FReply::Handled();
}

TSharedRef<SWidget> SUEAITextureGeneratorWindow::MakeSizeOptionWidget(TSharedPtr<FString> Option)
{
    return SNew(STextBlock).Text(FText::FromString(*Option));
}

void SUEAITextureGeneratorWindow::OnSizeSelectionChanged(TSharedPtr<FString> NewSelection, ESelectInfo::Type SelectInfo)
{
    if (NewSelection.IsValid())
    {
        SelectedSize = *NewSelection.Get();
    }
}

void SUEAITextureGeneratorWindow::SendOpenAIRequest(const FString& Prompt, const FString& TextureSize)
{
    // Create HTTP request
    FHttpModule* HttpModule = &FHttpModule::Get();
    CurrentRequest = HttpModule->CreateRequest();
    
    // Set URL and headers
    CurrentRequest->SetVerb(TEXT("POST"));
    CurrentRequest->SetURL(TEXT("https://api.openai.com/v1/images/generations"));
    CurrentRequest->SetHeader("Content-Type", "application/json");
    CurrentRequest->SetHeader("Authorization", FString::Printf(TEXT("Bearer %s"), *ApiKey));
    
    TSharedPtr<FJsonObject> JsonObject = MakeShared<FJsonObject>();
    JsonObject->SetStringField("prompt", Prompt);
    JsonObject->SetStringField("model", "dall-e-3");
    JsonObject->SetNumberField("n", 1);
    JsonObject->SetStringField("size", TextureSize);
    JsonObject->SetStringField("response_format", "b64_json");
    
    // Serialize request body
    FString RequestBody;
    TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&RequestBody);
    FJsonSerializer::Serialize(JsonObject.ToSharedRef(), Writer);
    
    // Set request body and callback
    CurrentRequest->SetContentAsString(RequestBody);
    CurrentRequest->OnProcessRequestComplete().BindRaw(this, &SUEAITextureGeneratorWindow::OnResponseReceived);
    
    // Send request
    CurrentRequest->ProcessRequest();
    UpdateProgress(0.3f);
}

void SUEAITextureGeneratorWindow::OnResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
    // Reset request handle
    CurrentRequest.Reset();
    
    if (!bWasSuccessful || !Response.IsValid())
    {
        SetUIState(true, 0.0f, TEXT("Failed to connect to OpenAI API"));
        return;
    }
    
    UpdateProgress(0.9f);
    
    // Parse response
    TSharedPtr<FJsonObject> JsonObject;
    TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(Response->GetContentAsString());
    
    if (!FJsonSerializer::Deserialize(Reader, JsonObject) || !JsonObject.IsValid())
    {
        SetUIState(true, 0.0f, TEXT("Failed to parse API response"));
        return;
    }

    // Uncomment for debugging respone
    //UE_LOG(LogTemp, Log, TEXT("Response: %s"), *Response->GetContentAsString());
    
    // Check for error
    if (JsonObject->HasField(TEXT("error")))
    {
        TSharedPtr<FJsonObject> ErrorObject = JsonObject->GetObjectField(TEXT("error"));
        FString ErrorMessage = ErrorObject->GetStringField(TEXT("message"));
        UE_LOG(LogTemp, Error, TEXT("Custom Print Error%s"), *ErrorMessage);
        SetUIState(true, 0.0f, TEXT("API Error: ") + ErrorMessage);
        return;
    }
    
    // Extract image data
    const TArray<TSharedPtr<FJsonValue>>* DataArray;
    if (!JsonObject->TryGetArrayField(TEXT("data"), DataArray) || DataArray->Num() == 0)
    {
        SetUIState(true, 0.0f, TEXT("No image data in response"));
        return;
    }
    
    UpdateProgress(0.8f);
    
    // Get base64 encoded image
    TSharedPtr<FJsonObject> ImageObject = (*DataArray)[0]->AsObject();
    FString Base64Image = ImageObject->GetStringField(TEXT("b64_json"));
    
    // Decode base64 image
    TArray<uint8> ImageData;
    if (!FBase64::Decode(Base64Image, ImageData))
    {
        SetUIState(true, 0.0f, TEXT("Failed to decode image data"));
        return;
    }
    
    // Save the texture
    SaveGeneratedImageAsTexture2D(ImageData);
}

void SUEAITextureGeneratorWindow::SetUIState(bool bEnabled, float ProgressValue, const FString& StatusMessage)
{
    SetUIEnabled(bEnabled);
    UpdateProgress(ProgressValue);
    UpdateStatus(StatusMessage);
}

void SUEAITextureGeneratorWindow::SetUIEnabled(bool bEnabled)
{
    if (PromptTextBox.IsValid())
    {
        PromptTextBox->SetEnabled(bEnabled);
    }
    
    if (SizeComboBox.IsValid())
    {
        SizeComboBox->SetEnabled(bEnabled);
    }

    if (GenerateButton.IsValid())
    {
        GenerateButton->SetEnabled(bEnabled);
    }
}

void SUEAITextureGeneratorWindow::UpdateProgress(float ProgressValue)
{
    if (ProgressBar.IsValid())
    {
        ProgressBar->SetPercent(ProgressValue);
    }
}

void SUEAITextureGeneratorWindow::UpdateStatus(const FString& StatusMessage)
{
    if (StatusText.IsValid())
    {
        StatusText->SetText(FText::FromString(StatusMessage));
    }
}

void SUEAITextureGeneratorWindow::SaveGeneratedImageAsTexture2D(const TArray<uint8>& ImageData)
{
    // Generate a unique asset name based on timestamp
    FDateTime Now = FDateTime::Now();
    FString AssetName = FString::Printf(TEXT("%s_%d%02d%02d_%02d%02d%02d"), 
        TEXT("AI_Texture_"),
        Now.GetYear(), Now.GetMonth(), Now.GetDay(),
        Now.GetHour(), Now.GetMinute(), Now.GetSecond());
    
    // Save as PNG in Content/AI_Textures
    FString TempDir = FPaths::ProjectContentDir() + TEXT("AI_Textures/");
    FString FileName = AssetName + TEXT(".png");
    FString FilePath = TempDir + FileName;

    // Ensure the directory exists
    IPlatformFile& PlatformFile = FPlatformFileManager::Get().GetPlatformFile();
    if (!PlatformFile.DirectoryExists(*TempDir))
    {
        PlatformFile.CreateDirectoryTree(*TempDir);
    }
    
    // Save the PNG data directly to file
    if (ImageData.Num() > 0)
    {
        if (FFileHelper::SaveArrayToFile(ImageData, *FilePath))
        {
            UE_LOG(LogTemp, Log, TEXT("PNG file saved successfully at: %s"), *FilePath);
        }
        else
        {
            UE_LOG(LogTemp, Error, TEXT("Failed to save PNG file at: %s"), *FilePath);
            return;
        }
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("Image data is empty"));
        return;
    }

    // Create a Texture2D asset from the PNG
    FString PackagePath = TEXT("/Game/AI_Textures/") + AssetName;
    FString PackageFilename = FPackageName::LongPackageNameToFilename(PackagePath, FPackageName::GetAssetPackageExtension());
    
    // Create package and factory
    UPackage* Package = CreatePackage(*PackagePath);
    UTextureFactory* TextureFactory = NewObject<UTextureFactory>();
    TextureFactory->AddToRoot();
    
    // Configure the factory
    TextureFactory->bCreateMaterial = false;
    TextureFactory->bEditAfterNew = false;
    TextureFactory->CompressionSettings = TC_Default;
    TextureFactory->MipGenSettings = TMGS_FromTextureGroup;
    
    // Load the PNG into a texture
    bool bOperationCanceled = false;
    UTexture2D* NewTexture = Cast<UTexture2D>(TextureFactory->FactoryCreateFile(
        UTexture2D::StaticClass(),
        Package,
        *AssetName,
        RF_Standalone | RF_Public,
        FilePath,
        TEXT("png"),
        GWarn,
        bOperationCanceled
    ));
    
    if (NewTexture)
    {
        // Configure the texture
        NewTexture->SRGB = true;
        NewTexture->CompressionSettings = TC_Default;
        NewTexture->MipGenSettings = TMGS_FromTextureGroup;
        
        // Notify the asset registry
        FAssetRegistryModule::AssetCreated(NewTexture);
        
        // Mark the package dirty and save it
        Package->MarkPackageDirty();
        UPackage::SavePackage(
            Package,
            NewTexture,
            EObjectFlags::RF_Public | EObjectFlags::RF_Standalone,
            *PackageFilename,
            GError, nullptr, true, true, SAVE_NoError
        );

        SetUIState(true, 1.0f, TEXT("Texture2D asset created successfully"));
        UE_LOG(LogTemp, Log, TEXT("Texture2D asset created successfully at: %s"), *PackagePath);
        
        // Remove the factory from root
        TextureFactory->RemoveFromRoot();
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to create Texture2D asset"));
        TextureFactory->RemoveFromRoot();
    }
}

END_SLATE_FUNCTION_BUILD_OPTIMIZATION

#undef LOCTEXT_NAMESPACE