#include "DolphinInstance.h"

#include "DolphinUnreal.h"
#include "Interfaces/IPluginManager.h"
#include "Misc/Guid.h"
#include "Misc/MonitoredProcess.h"
#include "Misc/Paths.h"

#include "AssetTypes/IsoAsset.h"
#include "FrameInput.h"
#include "Platform/WindowsSimpleProcessSpawner.h"

// STL
#include <string>

UDolphinInstance::UDolphinInstance(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
    FEditorDelegates::PausePIE.AddUObject(this, &UDolphinInstance::PausePIE);
}

void UDolphinInstance::Initialize(UIsoAsset* InIsoAsset, const FDolphinGraphicsSettings& InGraphicsSettings, const FDolphinRuntimeSettings& InRuntimeSettings)
{
    LaunchInstance(InIsoAsset, InGraphicsSettings, InRuntimeSettings);
}

UDolphinInstance::~UDolphinInstance()
{
    Terminate();
}

void UDolphinInstance::PausePIE(const bool bIsSimulating)
{
    DolphinIpcToInstanceData ipcData;
    std::shared_ptr<ToInstanceParams_StopRecordingInput> data = std::make_shared<ToInstanceParams_StopRecordingInput>();
    ipcData._call = DolphinInstanceIpcCall::DolphinInstance_StopRecordingInput;
    ipcSendToInstance(ipcData);
}

void UDolphinInstance::Tick(float DeltaTime)
{
    updateIpcListen();

    // Send a heartbeat to the running instance
    DolphinIpcToInstanceData ipcData;
    std::shared_ptr<ToInstanceParams_Heartbeat> data = std::make_shared<ToInstanceParams_Heartbeat>();
    ipcData._call = DolphinInstanceIpcCall::DolphinInstance_Heartbeat;
    ipcSendToInstance(ipcData);
}

void UDolphinInstance::DolphinServer_OnInstanceConnected(const ToServerParams_OnInstanceConnected& OnInstanceConnectedParams)
{
    if (GEngine)
    {
        GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Dolphin instance connected"));
    }
}

void UDolphinInstance::DolphinServer_OnInstanceHeartbeatAcknowledged(const ToServerParams_OnInstanceHeartbeatAcknowledged& onInstanceHeartbeatAcknowledgedParams)
{

}

void UDolphinInstance::DolphinServer_OnInstanceTerminated(const ToServerParams_OnInstanceTerminated& OnInstanceTerminatedParams)
{
    if (GEngine)
    {
        GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Dolphin instance terminated"));
    }
}

void UDolphinInstance::DolphinServer_OnInstanceRecordingStopped(const ToServerParams_OnInstanceRecordingStopped& onInstanceRecordingStopped)
{

}

void UDolphinInstance::LaunchInstance(UIsoAsset* InIsoAsset, const FDolphinGraphicsSettings& InGraphicsSettings, const FDolphinRuntimeSettings& InRuntimeSettings)
{
    static FString PluginContentDirectory = FPaths::ConvertRelativePathToFull(IPluginManager::Get().FindPlugin(TEXT("DolphinUnreal"))->GetContentDir());
    static FString ProjectContentDirectory = FPaths::ConvertRelativePathToFull(FPaths::ProjectContentDir());

    InstanceId = FGuid::NewGuid().ToString();
    FString DolphinBinaryFolder = FPaths::Combine(PluginContentDirectory, TEXT("Dolphin/"));
    FString DolphinBinaryPath = FPaths::Combine(DolphinBinaryFolder, TEXT("DolphinInstance.exe"));
    FString GamePath = InIsoAsset->Path;
    FString UserPath = FPaths::Combine(ProjectContentDirectory, "Dolphin");
    FString Params = FString::Format(TEXT("\"{0}\" -u \"{1}\" -p win32 -i {2}"), { GamePath, UserPath, InstanceId });
    FString OptionalWorkingDirectory = DolphinBinaryFolder;

    initializeChannels(std::string(TCHAR_TO_UTF8(*InstanceId)), false);

    FWindowsSimpleProcessSpawner::CreateProc(
        *DolphinBinaryPath,
        *Params,
        (OptionalWorkingDirectory != "") ? *OptionalWorkingDirectory : nullptr
    );
}

void UDolphinInstance::RequestLoadSaveState(FString SaveName)
{

}

void UDolphinInstance::RequestCreateSaveState(FString SaveName)
{

}

void UDolphinInstance::RequestPause()
{
    bIsPaused = true;

    DolphinIpcToInstanceData ipcData;
    std::shared_ptr<ToInstanceParams_PauseEmulation> data = std::make_shared<ToInstanceParams_PauseEmulation>();
    ipcData._call = DolphinInstanceIpcCall::DolphinInstance_PauseEmulation;
    ipcSendToInstance(ipcData);
}

void UDolphinInstance::RequestUnpause()
{
    bIsPaused = false;

    DolphinIpcToInstanceData ipcData;
    std::shared_ptr<ToInstanceParams_UnpauseEmulation> data = std::make_shared<ToInstanceParams_UnpauseEmulation>();
    ipcData._call = DolphinInstanceIpcCall::DolphinInstance_UnpauseEmulation;
    ipcSendToInstance(ipcData);
}

bool UDolphinInstance::IsPaused() const
{
    return bIsPaused;
}

void UDolphinInstance::RequestStartRecording()
{
    bIsRecordingInput = true;

    DolphinIpcToInstanceData ipcData;
    std::shared_ptr<ToInstanceParams_StartRecordingInput> data = std::make_shared<ToInstanceParams_StartRecordingInput>();
    ipcData._call = DolphinInstanceIpcCall::DolphinInstance_StartRecordingInput;
    ipcSendToInstance(ipcData);
}

void UDolphinInstance::RequestStopRecording()
{
    bIsRecordingInput = false;

    DolphinIpcToInstanceData ipcData;
    std::shared_ptr<ToInstanceParams_StopRecordingInput> data = std::make_shared<ToInstanceParams_StopRecordingInput>();
    ipcData._call = DolphinInstanceIpcCall::DolphinInstance_StopRecordingInput;
    ipcSendToInstance(ipcData);
}

bool UDolphinInstance::IsRecording() const
{
    return bIsRecordingInput;
}

void UDolphinInstance::RequestPlayInputs(UDataTable* FrameInputsTable)
{
    if (FrameInputsTable == nullptr)
    {
        return;
    }

    TArray<FFrameInput*> FrameInputs;
    FString ContextString = TEXT("PlayInputs");

    FrameInputsTable->GetAllRows(ContextString, FrameInputs);

    DolphinIpcToInstanceData ipcData;
    std::shared_ptr<ToInstanceParams_StopRecordingInput> data = std::make_shared<ToInstanceParams_StopRecordingInput>();
    ipcData._call = DolphinInstanceIpcCall::DolphinInstance_StopRecordingInput;
    ipcSendToInstance(ipcData);
}

void UDolphinInstance::Terminate()
{
    DolphinIpcToInstanceData ipcData;
    std::shared_ptr<ToInstanceParams_Terminate> data = std::make_shared<ToInstanceParams_Terminate>();
    ipcData._call = DolphinInstanceIpcCall::DolphinInstance_Terminate;
    ipcSendToInstance(ipcData);

    ConditionalBeginDestroy();
}
