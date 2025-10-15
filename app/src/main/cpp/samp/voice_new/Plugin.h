#pragma once

#include "include/util/Render.h"
#include "include/util/Samp.h"

#include "ControlPacket.h"
#include "Stream.h"
#include "Header.h"

class Plugin {
    Plugin() = delete;
    ~Plugin() = delete;
    Plugin(const Plugin&) = delete;
    Plugin(Plugin&&) = delete;
    Plugin& operator=(const Plugin&) = delete;
    Plugin& operator=(Plugin&&) = delete;

public:
    static bool OnPluginLoad() noexcept;
    static bool OnSampLoad() noexcept;

    //static bool recordStatus;
    static int MicRecord;
    static int MicPress;

private:
    static void OnInitGame() noexcept;
    static void OnExitGame() noexcept;

    static void MainLoop();

    static void ConnectHandler(const std::string& serverIp, uint16_t serverPort);
    static void PluginConnectHandler(SV::ConnectPacket& connectStruct);
    static bool PluginInitHandler(const SV::PluginInitPacket& initPacket);
     static void ControlPacketHandler(const ControlPacket& controlPacket);
    static void DisconnectHandler();

    static void OnDeviceInit();
    static void OnRender();
    static void OnDeviceFree();

private:
    static bool muteStatus;
    static bool recordStatus;
    static bool recordBusy;

    static std::map<uint32_t, StreamPtr> streamTable;
};