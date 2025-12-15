// SCP: 5K Copyright 2019 - 2025 Affray LLC

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"

THIRD_PARTY_INCLUDES_START
#include "YarnSpinnerCore/VirtualMachine.h"
#include "YarnSpinnerCore/Library.h"
#include "YarnSpinnerCore/Common.h"
THIRD_PARTY_INCLUDES_END

#include "YarnDialogueComponent.generated.h"

DECLARE_DELEGATE(FYarnDialogueRunnerContinueDelegate);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FYarnDialogueOnRunLineDelegate, class ULine*, Line, const TArray<TSoftObjectPtr<UObject>>&, LineAssets);
DECLARE_DYNAMIC_DELEGATE_RetVal_TwoParams(bool, FYarnDialogueOnRunCommandDelegate, const FString&, Command, const TArray<FString>&, Parameters);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FYarnDialogueOnRunOptionsDelegate, const TArray<class UOption*>&, Options);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FYarnDialogueStartedDelegate);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FYarnDialogueEndedDelegate);


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class YARNSPINNER_API UYarnDialogueComponent : public UActorComponent, public Yarn::ILogger, public Yarn::IVariableStorage
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UYarnDialogueComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

    UFUNCTION(BlueprintNativeEvent, Category="Yarn|Dialogue Runner")
    void OnDialogueStarted();
    
    UFUNCTION(BlueprintNativeEvent, Category="Yarn|Dialogue Runner")
    void OnDialogueEnded();
    
    UFUNCTION(BlueprintNativeEvent, Category="Yarn|Dialogue Runner")
    void OnRunLine(class ULine* Line, const TArray<TSoftObjectPtr<UObject>>& LineAssets);

    UFUNCTION(BlueprintNativeEvent, Category="Yarn|Dialogue Runner")
    void OnRunOptions(const TArray<class UOption*>& Options);

    UFUNCTION(BlueprintNativeEvent, Category="Yarn|Dialogue Runner")
    void OnRunCommand(const FString& Command, const TArray<FString>& Parameters);
    
    UFUNCTION(BlueprintCallable, Category="Yarn|Dialogue Runner")
    void StartDialogue(FName NodeName);
    
    UFUNCTION(BlueprintCallable, Category="Yarn|Dialogue Runner")
    void ContinueDialogue();

    // TODO: add StopDialogue() blueprint callback
    
    UFUNCTION(BlueprintCallable, Category="Yarn|Dialogue Runner")
    void SelectOption(UOption* Option);
    
    UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category="Dialogue Runner")
    UYarnProject* YarnProject;

    UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category="Dialogue Runner")
    bool bRunSelectedOptionsAsLines = false;
	
	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category="Yarn|Dialogue Runner")
	FYarnDialogueOnRunLineDelegate OnRunLineDelegate;
	UPROPERTY(BlueprintReadWrite, Category="Yarn|Dialogue Runner")
	FYarnDialogueOnRunCommandDelegate OnRunCommandDelegate;
	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category="Yarn|Dialogue Runner")
	FYarnDialogueOnRunOptionsDelegate OnRunOptionsDelegate;
	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category="Yarn|Dialogue Runner")
	FYarnDialogueStartedDelegate OnDialogueStartedDelegate;
	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category="Yarn|Dialogue Runner")
	FYarnDialogueEndedDelegate OnDialogueEndedDelegate;
	
	virtual void GetDisplayTextForLine(class ULine* Line, const Yarn::Line& YarnLine);
	
	bool bAutoContinue = false;

private:
    TUniquePtr<Yarn::VirtualMachine> VirtualMachine;

    TUniquePtr<Yarn::Library> Library;

    FYarnDialogueRunnerContinueDelegate ContinueDelegate;
	
	virtual void InitializeComponent() override;

    // ILogger
    virtual void Log(std::string Message, Type Severity = Type::INFO) override;

    // IVariableStorage
    virtual void SetValue(std::string Name, bool bValue) override;
    virtual void SetValue(std::string Name, float Value) override;
    virtual void SetValue(std::string Name, std::string Value) override;

    virtual bool HasValue(std::string Name) override;
    virtual Yarn::Value GetValue(std::string Name) override;

    virtual void ClearValue(std::string Name) override;

    FString GetLine(FName LineID, FName Language);

    UPROPERTY()
    FString Blah;

    class UYarnSubsystem* YarnSubsystem() const;
};