#pragma once

// for native audio
#include <SLES/OpenSLES.h>
#include <SLES/OpenSLES_Android.h>
#include <SLES/OpenSLES_AndroidConfiguration.h>

#include <array>

#include "SpeexCompat.h"

#include "include/util/Memory.hpp"

#include "Header.h"
#include "RecordBuffer.h"

class Record {
    Record() = delete;
    ~Record() = delete;
    Record(const Record&) = delete;
    Record(Record&&) = delete;
    Record& operator=(const Record&) = delete;
    Record& operator=(Record&&) = delete;

public:
    static bool Init(uint32_t bitrate) noexcept;
    static bool IsInited() noexcept;
    static void Free() noexcept;

    static void Tick() noexcept;

    static bool HasMicro() noexcept;

    static bool StartRecording() noexcept;
    static bool IsRecording() noexcept;
    static void StopRecording() noexcept;

    static bool StartChecking() noexcept;
    static bool IsChecking() noexcept;
    static void StopChecking() noexcept;

    static uint32_t GetFrame(uint8_t* bufferPtr, uint32_t bufferSize) noexcept;

    static bool GetMicroEnable() noexcept;
    static int GetMicroVolume() noexcept;
    static int GetMicroDevice() noexcept;

    static void SetMicroEnable(bool microEnable) noexcept;
    static void SetMicroVolume(int microVolume) noexcept;
    static void SetMicroDevice(int deviceIndex) noexcept;

    static void SyncConfigs() noexcept;
    static void ResetConfigs() noexcept;

    static const std::vector<std::string>& GetDeviceNamesList() noexcept;
    static const std::vector<int>& GetDeviceNumbersList() noexcept;

public:
    static HRECORD recordChannel;
    static OpusEncoder* encoder;
    static std::array<opus_int16, SV::kFrameSizeInSamples> encBuffer;
    static HSTREAM checkChannel;

    static SpeexEchoState *speexEchoState;
    static SpeexPreprocessState *speexPreprocessState;

    static bool recordingStopped;

private:
    static bool initStatus;

    static bool checkStatus;
    static bool recordStatus;

    static int usedDeviceIndex;
    static std::vector<std::string> deviceNamesList;
    static std::vector<int> deviceNumbersList;
};
