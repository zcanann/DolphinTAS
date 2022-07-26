#include "DolphinUnrealBlueprintLibrary.h"

#include "DolphinUnreal.h"

#include "DolphinInstance.h"

UDolphinUnrealBlueprintLibrary::UDolphinUnrealBlueprintLibrary(const FObjectInitializer& ObjectInitializer)
    : Super(ObjectInitializer)
{

}

UDolphinInstance* UDolphinUnrealBlueprintLibrary::CreateDolphinInstance(bool bRegisterAsDefaultInstance, UIsoAsset* IsoAsset, FDolphinGraphicsSettings GraphicsSettings, FDolphinRuntimeSettings RuntimeSettings)
{
    FDolphinUnrealModule& DolphinUnreal = FModuleManager::GetModuleChecked<FDolphinUnrealModule>(FDolphinUnrealModule::ModuleName);
    UDolphinInstance* Instance = DolphinUnreal.CreateNewInstance();

    Instance->Initialize(IsoAsset, GraphicsSettings, RuntimeSettings);

    if (bRegisterAsDefaultInstance)
    {
        DolphinUnreal.SetDefaultDolphinInstance(Instance);
    }

    return Instance;
}

UDolphinInstance* UDolphinUnrealBlueprintLibrary::GetDefaultDolphinInstance()
{
    FDolphinUnrealModule& DolphinUnreal = FModuleManager::GetModuleChecked<FDolphinUnrealModule>(FDolphinUnrealModule::ModuleName);
    UDolphinInstance* Instance =  DolphinUnreal.GetDefaultDolphinInstance();

    return Instance;
}

TArray<UDolphinInstance*> UDolphinUnrealBlueprintLibrary::GetDolphinInstances()
{
    FDolphinUnrealModule& DolphinUnreal = FModuleManager::GetModuleChecked<FDolphinUnrealModule>(FDolphinUnrealModule::ModuleName);
    TArray<UDolphinInstance*> Instances = DolphinUnreal.GetDolphinInstances();

    return Instances;
}

void UDolphinUnrealBlueprintLibrary::LoadSaveState(FString SaveName, UDolphinInstance* DolphinInstance)
{
    DolphinInstance = DolphinInstance ? DolphinInstance : GetDefaultDolphinInstance();

    if (DolphinInstance != nullptr)
    {
        DolphinInstance->RequestLoadSaveState(SaveName);
    }
}

void UDolphinUnrealBlueprintLibrary::CreateSaveState(FString SaveName, UDolphinInstance* DolphinInstance)
{
    DolphinInstance = DolphinInstance ? DolphinInstance : GetDefaultDolphinInstance();

    if (DolphinInstance != nullptr)
    {
        DolphinInstance->RequestCreateSaveState(SaveName);
    }
}

void UDolphinUnrealBlueprintLibrary::PlayInputs(UDataTable* FrameInputsTable, UDolphinInstance* DolphinInstance)
{
    DolphinInstance = DolphinInstance ? DolphinInstance : GetDefaultDolphinInstance();

    if (DolphinInstance != nullptr)
    {
        DolphinInstance->RequestPlayInputs(FrameInputsTable);
    }
}

/*
void UDolphinUnrealBlueprintLibrary::WaitFrames(int32 Frames, UDolphinInstance* DolphinInstance)
{
    DolphinInstance = DolphinInstance ? DolphinInstance : GetDefaultDolphinInstance();

    if (DolphinInstance == nullptr)
    {
        return;
    }
}

void UDolphinUnrealBlueprintLibrary::PressA(int32 Frames, UDolphinInstance* DolphinInstance)
{
    DolphinInstance = DolphinInstance ? DolphinInstance : GetDefaultDolphinInstance();

    if (DolphinInstance == nullptr)
    {
        return;
    }
}

void UDolphinUnrealBlueprintLibrary::PressB(int32 Frames, UDolphinInstance* DolphinInstance)
{
    DolphinInstance = DolphinInstance ? DolphinInstance : GetDefaultDolphinInstance();

    if (DolphinInstance == nullptr)
    {
        return;
    }
}

void UDolphinUnrealBlueprintLibrary::PressX(int32 Frames, UDolphinInstance* DolphinInstance)
{
    DolphinInstance = DolphinInstance ? DolphinInstance : GetDefaultDolphinInstance();

    if (DolphinInstance == nullptr)
    {
        return;
    }
}

void UDolphinUnrealBlueprintLibrary::PressY(int32 Frames, UDolphinInstance* DolphinInstance)
{
    DolphinInstance = DolphinInstance ? DolphinInstance : GetDefaultDolphinInstance();

    if (DolphinInstance == nullptr)
    {
        return;
    }
}

void UDolphinUnrealBlueprintLibrary::PressZ(int32 Frames, UDolphinInstance* DolphinInstance)
{
    DolphinInstance = DolphinInstance ? DolphinInstance : GetDefaultDolphinInstance();

    if (DolphinInstance == nullptr)
    {
        return;
    }
}

void UDolphinUnrealBlueprintLibrary::PressDPadLeft(int32 Frames, UDolphinInstance* DolphinInstance)
{
    DolphinInstance = DolphinInstance ? DolphinInstance : GetDefaultDolphinInstance();

    if (DolphinInstance == nullptr)
    {
        return;
    }
}

void UDolphinUnrealBlueprintLibrary::PressDPadRight(int32 Frames, UDolphinInstance* DolphinInstance)
{
    DolphinInstance = DolphinInstance ? DolphinInstance : GetDefaultDolphinInstance();

    if (DolphinInstance == nullptr)
    {
        return;
    }
}

void UDolphinUnrealBlueprintLibrary::PressDPadUp(int32 Frames, UDolphinInstance* DolphinInstance)
{
    DolphinInstance = DolphinInstance ? DolphinInstance : GetDefaultDolphinInstance();

    if (DolphinInstance == nullptr)
    {
        return;
    }
}

void UDolphinUnrealBlueprintLibrary::PressDPadDown(int32 Frames, UDolphinInstance* DolphinInstance)
{
    DolphinInstance = DolphinInstance ? DolphinInstance : GetDefaultDolphinInstance();

    if (DolphinInstance == nullptr)
    {
        return;
    }
}

void UDolphinUnrealBlueprintLibrary::PressR(bool bInIsPressed, uint8 InAnalogAmount, int32 Frames, UDolphinInstance* DolphinInstance)
{
    DolphinInstance = DolphinInstance ? DolphinInstance : GetDefaultDolphinInstance();

    if (DolphinInstance == nullptr)
    {
        return;
    }
}

void UDolphinUnrealBlueprintLibrary::PressL(bool bInIsPressed, uint8 InAnalogAmount, int32 Frames, UDolphinInstance* DolphinInstance)
{
    DolphinInstance = DolphinInstance ? DolphinInstance : GetDefaultDolphinInstance();

    if (DolphinInstance == nullptr)
    {
        return;
    }
}

void UDolphinUnrealBlueprintLibrary::AnalogStickX(uint8 AnalogAmount, int32 Frames, UDolphinInstance* DolphinInstance)
{
    DolphinInstance = DolphinInstance ? DolphinInstance : GetDefaultDolphinInstance();

    if (DolphinInstance == nullptr)
    {
        return;
    }
}

void UDolphinUnrealBlueprintLibrary::AnalogStickY(uint8 AnalogAmount, int32 Frames, UDolphinInstance* DolphinInstance)
{
    DolphinInstance = DolphinInstance ? DolphinInstance : GetDefaultDolphinInstance();

    if (DolphinInstance == nullptr)
    {
        return;
    }
}

void UDolphinUnrealBlueprintLibrary::CStickY(uint8 AnalogAmount, int32 Frames, UDolphinInstance* DolphinInstance)
{
    DolphinInstance = DolphinInstance ? DolphinInstance : GetDefaultDolphinInstance();

    if (DolphinInstance == nullptr)
    {
        return;
    }
}

void UDolphinUnrealBlueprintLibrary::CStickX(uint8 AnalogAmount, int32 Frames, UDolphinInstance* DolphinInstance)
{
    DolphinInstance = DolphinInstance ? DolphinInstance : GetDefaultDolphinInstance();

    if (DolphinInstance == nullptr)
    {
        return;
    }
}
*/
