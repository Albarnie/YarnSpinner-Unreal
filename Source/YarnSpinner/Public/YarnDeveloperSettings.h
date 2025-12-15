// SCP: 5K Copyright 2019 - 2025 Affray LLC

#pragma once

#include "CoreMinimal.h"
#include "DeveloperSettings.h"
#include "YarnDeveloperSettings.generated.h"

/**
 * 
 */
UCLASS()
class YARNSPINNER_API UYarnDeveloperSettings : public UDeveloperSettings
{
	GENERATED_BODY()
	
public:
	//Base path to search for blueprints to track from yarn
	//Try to set this to a folder that contains just blueprints, and as few art assets as possible.
	UPROPERTY(EditAnywhere, meta=(RelativePath))
	FDirectoryPath BlueprintImportPath = {TEXT("/Game/YarnSpinner/")};
};
