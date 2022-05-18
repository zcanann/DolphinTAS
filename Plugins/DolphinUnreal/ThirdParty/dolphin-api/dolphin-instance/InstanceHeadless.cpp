// Copyright 2018 Dolphin Emulator Project
// SPDX-License-Identifier: GPL-2.0-or-later

#include "Instance.h"

#include "Core/Core.h"

#include <cstdio>
#include <thread>
#include <string>

class InstanceHeadless : public Instance
{
public:
    InstanceHeadless(const std::string& channelId);

    void SetTitle(const std::string& title) override;
    void MainLoop() override;

    WindowSystemInfo GetWindowSystemInfo() const override;
};

InstanceHeadless::InstanceHeadless(const std::string& channelId) : Instance(channelId)
{
}

void InstanceHeadless::SetTitle(const std::string& title)
{
    std::fprintf(stdout, "%s\n", title.c_str());
}

void InstanceHeadless::MainLoop()
{
    while (_running.IsSet())
    {
        UpdateRunningFlag();
        Core::HostDispatchJobs();
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
}

WindowSystemInfo InstanceHeadless::GetWindowSystemInfo() const
{
    WindowSystemInfo wsi;
    wsi.type = WindowSystemType::Headless;
    wsi.display_connection = nullptr;
    wsi.render_window = nullptr;
    wsi.render_surface = nullptr;
    return wsi;
}

std::unique_ptr<Instance> Instance::CreateHeadlessInstance(const std::string& channelId)
{
    return std::make_unique<InstanceHeadless>(channelId);
}