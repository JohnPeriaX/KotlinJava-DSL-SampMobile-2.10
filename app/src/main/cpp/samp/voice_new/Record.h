#pragma once

#include <array>

#include "include/util/Memory.hpp"

#include "Header.h"

class Record {
    Record() = delete;
    ~Record() = delete;
    Record(const Record&) = delete;
    Record(Record&&) = delete;
    Record& operator=(const Record&) = delete;
    Record& operator=(Record&&) = delete;

public:
    static bool Init(uint32_t bitrate) noexcept;
    static void Free() noexcept;

    static void Tick() noexcept;

    static bool HasMicro() noexcept;

    static bool StartRecording() noexcept;
    static bool IsRecording() noexcept;
    static void StopRecording() noexcept;

    static void StopChecking() noexcept;

    static uint32_t GetFrame(uint8_t* bufferPtr, uint32_t bufferSize) noexcept;

    static void SetMicroEnable(bool microEnable) noexcept;
    static void SetMicroVolume(int microVolume) noexcept;

    static void SyncConfigs() noexcept;
    static void ResetConfigs() noexcept;

private:
    static bool initStatus;

    static bool checkStatus;
    static bool recordStatus;

    static HRECORD recordChannel;
    static OpusEncoder* encoder;
    static std::array<opus_int16, SV::kFrameSizeInSamples> encBuffer;
    static HSTREAM checkChannel;

    static int usedDeviceIndex;
    static std::vector<std::string> deviceNamesList;
    static std::vector<int> deviceNumbersList;
};
