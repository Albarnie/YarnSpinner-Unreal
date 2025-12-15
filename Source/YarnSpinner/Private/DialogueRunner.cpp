// Fill out your copyright notice in the Description page of Project Settings.


#include "DialogueRunner.h"
#include "Line.h"
#include "Option.h"
#include "YarnDialogueComponent.h"
#include "YarnSubsystem.h"
#include "YarnSpinner.h"
#include "Kismet/KismetInternationalizationLibrary.h"
#include "Misc/YSLogging.h"

THIRD_PARTY_INCLUDES_START
#include "YarnSpinnerCore/VirtualMachine.h"
THIRD_PARTY_INCLUDES_END
//#include "StaticParty.h"


// Sets default values
ADialogueRunner::ADialogueRunner()
{
    // Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;
	
	DialogueComponent = CreateDefaultSubobject<UYarnDialogueComponent>(TEXT("DialogueComponent"));
	
	DialogueComponent->OnDialogueEndedDelegate.AddDynamic(this, &ADialogueRunner::OnDialogueEnded);
	DialogueComponent->OnDialogueStartedDelegate.AddDynamic(this, &ADialogueRunner::OnDialogueStarted);
	DialogueComponent->OnRunLineDelegate.AddDynamic(this, &ADialogueRunner::OnRunLine);
	DialogueComponent->OnRunCommandDelegate.BindDynamic(this, &ADialogueRunner::OnRunCommandInternal);
}


// Called when the game starts or when spawned
void ADialogueRunner::PreInitializeComponents()
{
    Super::PreInitializeComponents();
}


// Called every frame
void ADialogueRunner::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}


void ADialogueRunner::OnDialogueStarted_Implementation()
{
    // default = no-op
}


void ADialogueRunner::OnDialogueEnded_Implementation()
{
    // default = no-op
}


void ADialogueRunner::OnRunLine_Implementation(ULine* Line, const TArray<TSoftObjectPtr<UObject>>& LineAssets)
{
    // default = log and immediately continue
    UE_LOG(LogYarnSpinner, Warning, TEXT("DialogueRunner received line with ID \"%s\". Implement OnRunLine to customise its behaviour."), *Line->LineID.ToString());
    ContinueDialogue();
}


void ADialogueRunner::OnRunOptions_Implementation(const TArray<class UOption*>& Options)
{
    // default = log and choose the first option
    UE_LOG(LogYarnSpinner, Warning, TEXT("DialogueRunner received %i options. Choosing the first one by default. Implement OnRunOptions to customise its behaviour."), Options.Num());

    SelectOption(Options[0]);
}


void ADialogueRunner::OnRunCommand_Implementation(const FString& Command, const TArray<FString>& Parameters)
{
    // default = no-op
    UE_LOG(LogYarnSpinner, Warning, TEXT("DialogueRunner received command \"%s\". Implement OnRunCommand to customise its behaviour."), *Command);
    ContinueDialogue();
}


/** Starts running dialogue from the given node name. */
void ADialogueRunner::StartDialogue(FName NodeName)
{
    DialogueComponent->StartDialogue(NodeName);
}


/** Continues running the current dialogue, producing either lines, options, commands, or a dialogue-end signal. */
void ADialogueRunner::ContinueDialogue()
{
    YS_LOG_FUNCSIG

    DialogueComponent->ContinueDialogue();
}


/** Indicates to the dialogue runner that an option was selected. */
void ADialogueRunner::SelectOption(UOption* Option)
{
    DialogueComponent->SelectOption(Option);
}


void ADialogueRunner::Log(std::string Message, Type Severity)
{
    FString MessageText = FString(UTF8_TO_TCHAR(Message.c_str()));

    switch (Severity)
    {
    case Type::INFO:
        YS_LOG("YarnSpinner: %s", *MessageText);
        break;
    case Type::WARNING:
        YS_WARN("YarnSpinner: %s", *MessageText);
        break;
    case Type::ERROR:
        YS_ERR("YarnSpinner: %s", *MessageText);
        break;
    }
}


void ADialogueRunner::SetValue(std::string Name, bool bValue)
{
    YS_LOG("Setting variable %s to bool %i", UTF8_TO_TCHAR(Name.c_str()), bValue)
    YarnSubsystem()->SetValue(Name, bValue);
}


void ADialogueRunner::SetValue(std::string Name, float Value)
{
    YS_LOG("Setting variable %s to float %f", UTF8_TO_TCHAR(Name.c_str()), Value)
    YarnSubsystem()->SetValue(Name, Value);
}


void ADialogueRunner::SetValue(std::string Name, std::string Value)
{
    YS_LOG("Setting variable %s to string %s", UTF8_TO_TCHAR(Name.c_str()), UTF8_TO_TCHAR(Value.c_str()))
    YarnSubsystem()->SetValue(Name, Value);
}


bool ADialogueRunner::HasValue(std::string Name)
{
    return YarnSubsystem()->HasValue(Name);
}


Yarn::Value ADialogueRunner::GetValue(std::string Name)
{
    Yarn::Value Value = YarnSubsystem()->GetValue(Name);
    YS_LOG("Retrieving variable %s with value %s", UTF8_TO_TCHAR(Name.c_str()), UTF8_TO_TCHAR(Value.ConvertToString().c_str()))
    return Value;
}


void ADialogueRunner::ClearValue(std::string Name)
{
    YS_LOG("Clearing variable %s", UTF8_TO_TCHAR(Name.c_str()))
    YarnSubsystem()->ClearValue(Name);
}


UYarnSubsystem* ADialogueRunner::YarnSubsystem() const
{
    if (!GetGameInstance())
    {
        YS_WARN("Could not retrieve YarnSubsystem because GetGameInstance() returned null")
        return nullptr;
    }
    return GetGameInstance()->GetSubsystem<UYarnSubsystem>();
}


void ADialogueRunner::GetDisplayTextForLine(ULine* Line, const Yarn::Line& YarnLine)
{
    DialogueComponent->GetDisplayTextForLine(Line, YarnLine);
}
bool ADialogueRunner::OnRunCommandInternal(const FString& Command, const TArray<FString>& Parameters)
{
	OnRunCommand(Command, Parameters);
	
	return true;
}