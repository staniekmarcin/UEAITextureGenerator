// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "UEAITextureGeneratorSettings.generated.h"

/**
 * 
 */
UCLASS(config=Editor, defaultconfig)
class UEAITEXTUREGENERATOR_API UUEAITextureGeneratorSettings : public UObject
{
	GENERATED_BODY()

public:
	// This value will be editable in the project settings UI
	UPROPERTY(EditAnywhere, config, Category = "OpenAI", meta=(DisplayName = "OpenAI API Key"))
	FString OpenAIAPIKey;
};
