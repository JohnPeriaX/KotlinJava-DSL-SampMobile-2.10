#include "../vendor/bass/bass.h"
#include <opus.h>

#include "Record.h"

#include "PluginConfig.h"
#include "../main.h"

// buffer queue player interfaces
static SLAndroidSimpleBufferQueueItf bqPlayerBufferQueue;

static SLObjectItf bqPlayerObject = NULL;
static SLPlayItf bqPlayerPlay;
static SLVolumeItf bqPlayerVolume;
static SLPlaybackRateItf bqPlayerPlayBack;

static SLObjectItf outputMixObject = NULL;

void bqPlayerCallback(SLAndroidSimpleBufferQueueItf bufferQueue, void *context)
{

}

bool Record::Init(const uint32_t bitrate) noexcept
{
    if(Record::initStatus)
        return false;

    if(BASS_IsStarted() == 0)
        return false;

    LogVoice("[sv:dbg:record:init] : module initializing...");

    Record::deviceNamesList.clear();
    Record::deviceNumbersList.clear();

    {
        BASS_DEVICEINFO devInfo {};

        for(int devNumber { 0 }; BASS_RecordGetDeviceInfo(devNumber, &devInfo); ++devNumber)
        {
            const bool deviceEnabled = devInfo.flags & BASS_DEVICE_ENABLED;
            const bool deviceLoopback = devInfo.flags & BASS_DEVICE_LOOPBACK;
            const uint32_t deviceType = devInfo.flags & BASS_DEVICE_TYPE_MASK;

            LogVoice("[sv:dbg:record:init] : device detect "
                "[ id(%d) enabled(%hhu) loopback(%hhu) name(%s) type(0x%x) ]",
                devNumber, deviceEnabled, deviceLoopback, devInfo.name != nullptr
                ? devInfo.name : "none", deviceType);

            if(deviceEnabled && !deviceLoopback && devInfo.name != nullptr)
            {
                try
                {
                    Record::deviceNumbersList.emplace_back(devNumber);
                    Record::deviceNamesList.emplace_back(devInfo.name);
                }
                catch(const std::exception& exception)
                {
                    LogVoice("[sv:err:record:init] : failed to add device");
                    return false;
                }
            }
        }
    }

    Memory::ScopeExit deviceListsResetScope { [] { Record::usedDeviceIndex = -1;
                                                   Record::deviceNamesList.clear();
                                                   Record::deviceNumbersList.clear(); } };

    if(Record::deviceNamesList.empty() || Record::deviceNumbersList.empty())
    {
        LogVoice("[sv:inf:record:init] : failed to find microphone");
        return false;
    }

    {
        int opusErrorCode { -1 };

        Record::encoder = opus_encoder_create(SV::kFrequency, 1,
            OPUS_APPLICATION_VOIP, &opusErrorCode);

        if(Record::encoder == nullptr || opusErrorCode < 0)
        {
            LogVoice("[sv:err:record:init] : failed to "
                "create encoder (code:%d)", opusErrorCode);
            return false;
        }
    }

    Memory::ScopeExit encoderResetScope { [] { opus_encoder_destroy(Record::encoder); } };

    if(const auto error = opus_encoder_ctl(Record::encoder,
        OPUS_SET_BITRATE(bitrate)); error < 0)
    {
        LogVoice("[sv:err:record:init] : failed to "
            "set bitrate for encoder (code:%d)", error);
        return false;
    }

    if(const auto error = opus_encoder_ctl(Record::encoder,
        OPUS_SET_SIGNAL(OPUS_SIGNAL_VOICE)); error < 0)
    {
        LogVoice("[sv:err:record:init] : failed to "
            "set audiosignal type for encoder (code:%d)", error);
        return false;
    }

    if(const auto error = opus_encoder_ctl(Record::encoder,
        OPUS_SET_COMPLEXITY(10)); error < 0)
    {
        LogVoice("[sv:err:record:init] : failed to "
            "set complexity for encoder (code:%d)", error);
        return false;
    }

    if(const auto error = opus_encoder_ctl(Record::encoder,
        OPUS_SET_PREDICTION_DISABLED(0)); error < 0)
    {
        LogVoice("[sv:err:record:init] : failed to "
            "enable prediction for encoder (code:%d)", error);
        return false;
    }

    if(const auto error = opus_encoder_ctl(Record::encoder,
        OPUS_SET_LSB_DEPTH(8)); error < 0)
    {
        LogVoice("[sv:err:record:init] : failed to "
            "set lsb depth for encoder (code:%d)", error);
        return false;
    }

    if(const auto error = opus_encoder_ctl(Record::encoder,
        OPUS_SET_FORCE_CHANNELS(1)); error < 0)
    {
        LogVoice("[sv:err:record:init] : failed to "
            "set count channels for encoder (code:%d)", error);
        return false;
    }

    if(const auto error = opus_encoder_ctl(Record::encoder,
        OPUS_SET_DTX(0)); error < 0)
    {
        LogVoice("[sv:err:record:init] : failed to "
            "set dtx for encoder (code:%d)", error);
        return false;
    }

    if(const auto error = opus_encoder_ctl(Record::encoder,
        OPUS_SET_INBAND_FEC(1)); error < 0)
    {
        LogVoice("[sv:err:record:init] : failed to "
            "set inband fec for encoder (code:%d)", error);
        return false;
    }

    if(const auto error = opus_encoder_ctl(Record::encoder,
        OPUS_SET_PACKET_LOSS_PERC(10)); error < 0)
    {
        LogVoice("[sv:err:record:init] : failed to set "
            "packet loss percent for encoder (code:%d)", error);
        return false;
    }

    Record::usedDeviceIndex = -1;

    if(PluginConfig::IsRecordLoaded() && !PluginConfig::GetDeviceName().empty())
    {
        for(std::size_t i { 0 }; i < Record::deviceNamesList.size(); ++i)
        {
            if(Record::deviceNamesList[i] == PluginConfig::GetDeviceName())
            {
                Record::usedDeviceIndex = i;
                break;
            }
        }
    }

    bool initRecordStatus = BASS_RecordInit(Record::usedDeviceIndex != -1 ?
                            Record::deviceNumbersList[Record::usedDeviceIndex] : -1);

    if(!initRecordStatus && Record::usedDeviceIndex != -1)
    {
        initRecordStatus = BASS_RecordInit(Record::usedDeviceIndex = -1);
    }

    if(initRecordStatus && Record::usedDeviceIndex == -1)
    {
        for(std::size_t i { 0 }; i < Record::deviceNumbersList.size(); ++i)
        {
            if(Record::deviceNumbersList[i] == BASS_RecordGetDevice())
            {
                Record::usedDeviceIndex = i;
                break;
            }
        }
    }

    Memory::ScopeExit recordResetScope { [] { BASS_RecordFree(); } };

    if(!initRecordStatus || Record::usedDeviceIndex == -1)
    {
        LogVoice("[sv:err:record:init] : failed to "
            "init device (code:%d)", BASS_ErrorGetCode());
        return false;
    }

    if(!PluginConfig::IsRecordLoaded())
    {
        PluginConfig::SetDeviceName(Record::deviceNamesList[Record::usedDeviceIndex]);
    }

    Record::recordChannel = BASS_RecordStart(SV::kFrequency2, 1,
        BASS_RECORD_PAUSE, nullptr, nullptr);

    if(Record::recordChannel == NULL)
    {
        LogVoice("[sv:err:record:init] : failed to create record "
            "stream (code:%d)", BASS_ErrorGetCode());
        return false;
    }

    Memory::ScopeExit channelResetScope { [] { BASS_ChannelStop(Record::recordChannel); } };

    Record::checkChannel = BASS_StreamCreate(SV::kFrequency2, 1,
        NULL, STREAMPROC_PUSH, nullptr);

    if(Record::checkChannel == NULL)
    {
        LogVoice("[sv:err:record:init] : failed to create "
            "check stream (code:%d)", BASS_ErrorGetCode());
        return false;
    }

    BASS_ChannelSetAttribute(Record::checkChannel, BASS_ATTRIB_VOL, 4.f);

    int iSamplingSize = 44100;

    Record::speexEchoState = speex_echo_state_init(SV::kFrameSizeInSamples, SV::kFrameSizeInBytes);
    speex_echo_ctl(Record::speexEchoState, SPEEX_ECHO_SET_SAMPLING_RATE, &iSamplingSize);

    speexPreprocessState = speex_preprocess_state_init(SV::kFrameSizeInSamples, iSamplingSize);
    speex_preprocess_ctl(speexPreprocessState, SPEEX_PREPROCESS_SET_ECHO_STATE, speexEchoState);

/*
    // create engine
    SLresult result = slCreateEngine(&engineObject, 0, NULL, 0, NULL, NULL);
    if(result != SL_RESULT_SUCCESS) 
    {
        LogVoice("[sv:err:record:init] : failed to create "
            "engine");
        return false;
    }

    // realize the engine
    result = (*engineObject)->Realize(engineObject, SL_BOOLEAN_FALSE);
    if(result != SL_RESULT_SUCCESS) 
    {
        LogVoice("[sv:err:record:init] : failed to realize "
            "engine");
        return false;
    }

    // create output mix, with environmental reverb specified as a non-required interface
    const SLInterfaceID idss[1] = { SL_IID_PLAYBACKRATE };
    const SLboolean reqss[1] = { SL_BOOLEAN_FALSE };
    result = (*engineEngine)->CreateOutputMix(engineEngine, &outputMixObject, 1, idss, reqss);
    assert(SL_RESULT_SUCCESS == result);

    // realize the output mix
    result = (*outputMixObject)->Realize(outputMixObject, SL_BOOLEAN_FALSE);
    assert(SL_RESULT_SUCCESS == result);

    // configure audio source
    SLDataLocator_AndroidSimpleBufferQueue loc_bufq = { SL_DATALOCATOR_ANDROIDSIMPLEBUFFERQUEUE, 2 };
    SLDataFormat_PCM format_pcm = { SL_DATAFORMAT_PCM, 1, SL_SAMPLINGRATE_44_1, SL_PCMSAMPLEFORMAT_FIXED_16, SL_PCMSAMPLEFORMAT_FIXED_16, SL_SPEAKER_FRONT_CENTER, SL_BYTEORDER_LITTLEENDIAN };
    SLDataSource audioSrcs = { &loc_bufq, &format_pcm };

    // configure audio sink
    SLDataLocator_OutputMix loc_outmix = { SL_DATALOCATOR_OUTPUTMIX, outputMixObject };
    SLDataSink audioSnks = { &loc_outmix, NULL };

    // create audio player
    const SLInterfaceID ids[4] = { SL_IID_PLAYBACKRATE, SL_IID_BUFFERQUEUE, SL_IID_VOLUME, SL_IID_ANDROIDCONFIGURATION };
    const SLboolean reqs[4] = { SL_BOOLEAN_TRUE, SL_BOOLEAN_TRUE, SL_BOOLEAN_TRUE, SL_BOOLEAN_TRUE };
    result = (*engineEngine)->CreateAudioPlayer(engineEngine, &bqPlayerObject, &audioSrcs, &audioSnks, 4, ids, reqs);
    assert(SL_RESULT_SUCCESS == result);

    // realize the player
    result = (*bqPlayerObject)->Realize(bqPlayerObject, SL_BOOLEAN_FALSE);
    assert(SL_RESULT_SUCCESS == result);

    // get the configure interface
    SLAndroidConfigurationItf playerConfig;
    result = (*bqPlayerObject)->GetInterface(bqPlayerObject, SL_IID_ANDROIDCONFIGURATION, &playerConfig);
    assert(SL_RESULT_SUCCESS == result);

    // get the play interface
    result = (*bqPlayerObject)->GetInterface(bqPlayerObject, SL_IID_PLAY, &bqPlayerPlay);
    assert(SL_RESULT_SUCCESS == result);

    // get the buffer queue interface
    result = (*bqPlayerObject)->GetInterface(bqPlayerObject, SL_IID_BUFFERQUEUE, &bqPlayerBufferQueue);
    assert(SL_RESULT_SUCCESS == result);

    // get the playback rate interface
    result = (*bqPlayerObject)->GetInterface(bqPlayerObject, SL_IID_PLAYBACKRATE, &bqPlayerPlayBack);
    assert(SL_RESULT_SUCCESS == result);

    // get the volume interface
    result = (*bqPlayerObject)->GetInterface(bqPlayerObject, SL_IID_VOLUME, &bqPlayerVolume);
    assert(SL_RESULT_SUCCESS == result);

    // set stream to voice call
    SLint32 streamType = SL_ANDROID_STREAM_VOICE;
    result = (*playerConfig)->SetConfiguration(playerConfig, SL_ANDROID_KEY_STREAM_TYPE, &streamType, sizeof(SLint32));
    assert(SL_RESULT_SUCCESS == result);

    // register callback on the buffer queue
    result = (*bqPlayerBufferQueue)->RegisterCallback(bqPlayerBufferQueue, bqPlayerCallback, NULL);
    assert(SL_RESULT_SUCCESS == result);

    // set the player's state to playing
    result = (*bqPlayerPlay)->SetPlayState(bqPlayerPlay, SL_PLAYSTATE_PLAYING);
    assert(SL_RESULT_SUCCESS == result);

    float fVolume = 1.f;

    // get min & max volume
    SLmillibel minVolume = SL_MILLIBEL_MIN;
    SLmillibel maxVolume = SL_MILLIBEL_MIN;
    result = (*bqPlayerVolume)->GetMaxVolumeLevel(bqPlayerVolume, &maxVolume);
    assert(SL_RESULT_SUCCESS == result);

    SLmillibel volume = minVolume + (SLmillibel)(((float)(maxVolume - minVolume)) * fVolume);

    // set the player's volume
    result = (*bqPlayerVolume)->SetVolumeLevel(bqPlayerVolume, volume);
    assert(SL_RESULT_SUCCESS == result);
*/
    if(!PluginConfig::IsRecordLoaded())
    {
        PluginConfig::SetRecordLoaded(true);
        Record::ResetConfigs();
    }

    deviceListsResetScope.Release();
    encoderResetScope.Release();
    recordResetScope.Release();
    channelResetScope.Release();

    LogVoice("[sv:dbg:record:init] : module initialized");

    Record::initStatus = true;
    Record::SyncConfigs();

    return true;
}

bool Record::IsInited() noexcept
{
    return Record::initStatus;
}

void Record::Free() noexcept
{
    if(!Record::initStatus)
        return;

    LogVoice("[sv:dbg:record:free] : module releasing...");

    Record::StopRecording();
    BASS_ChannelStop(Record::recordChannel);
    BASS_RecordFree();

    Record::StopChecking();
    BASS_StreamFree(Record::checkChannel);

    opus_encoder_destroy(Record::encoder);

    Record::usedDeviceIndex = -1;
    Record::deviceNumbersList.clear();
    Record::deviceNamesList.clear();

    LogVoice("[sv:dbg:record:free] : module released");

    Record::initStatus = false;
}

int clampEx(int source, int min, int max)
{
    if (source < min) return min;
    if (source > max) return max;
    return source;
}

void Record::Tick() noexcept
{
    if(!Record::initStatus || !Record::checkStatus)
        return;

    if(const auto bufferSize = BASS_ChannelGetData(Record::recordChannel,
        nullptr, BASS_DATA_AVAILABLE); bufferSize != -1 && bufferSize != 0)
    {
        if(const auto readDataSize = BASS_ChannelGetData(Record::recordChannel, Record::encBuffer.data(),
            clampEx(bufferSize, 0ul, SV::kFrameSizeInBytes)); readDataSize != -1 && readDataSize != 0)
        {
            std::array<opus_int16, SV::kFrameSizeInSamples> encOutBuffer {};

            // perform echo canceling
            speex_echo_capture(speexEchoState, Record::encBuffer.data(), encOutBuffer.data());
            
            // apply noise/echo suppresion
            speex_preprocess_run(speexPreprocessState, encOutBuffer.data());

            // playback the audio and reset echo canceller if we got underrun
            BASS_StreamPutData(Record::checkChannel, encOutBuffer.data(), readDataSize);
            //speex_echo_state_reset(speexEchoState);

            // put frame into playback buffer
            speex_echo_playback(speexEchoState, Record::encBuffer.data());

            //(*bqPlayerBufferQueue)->Enqueue(bqPlayerBufferQueue, Record::encBuffer.data(), readDataSize);
        }
    }
}

uint32_t Record::GetFrame(uint8_t* const bufferPtr, const uint32_t bufferSize) noexcept
{
    if(!Record::initStatus || !Record::recordStatus || Record::checkStatus)
        return NULL;

    //LogVoice("Stuck?");

    const auto cBufferSize = BASS_ChannelGetData(Record::recordChannel, nullptr, BASS_DATA_AVAILABLE);
    if(cBufferSize == -1 || cBufferSize < SV::kFrameSizeInBytes) return NULL;

    if(BASS_ChannelGetData(Record::recordChannel, Record::encBuffer.data(),
        SV::kFrameSizeInBytes) != SV::kFrameSizeInBytes) return NULL;

    std::array<opus_int16, SV::kFrameSizeInSamples> encOutBuffer {};

    // perform echo canceling
    speex_echo_capture(Record::speexEchoState, Record::encBuffer.data(), encOutBuffer.data());

    //LogVoice("Here?");

    const auto encDataLength = opus_encode(Record::encoder, encOutBuffer.data(),
        SV::kFrameSizeInSamples, bufferPtr, bufferSize);

    //LogVoice("Here!");

    return encDataLength > 0 ? static_cast<uint32_t>(encDataLength) : NULL;
}

bool Record::HasMicro() noexcept
{
    BASS_DEVICEINFO devInfo {};

    for(uint32_t devNumber { 0 }; BASS_RecordGetDeviceInfo(devNumber, &devInfo); ++devNumber)
    {
        const bool deviceEnabled = devInfo.flags & BASS_DEVICE_ENABLED;
        const bool deviceLoopback = devInfo.flags & BASS_DEVICE_LOOPBACK;

        if(deviceEnabled && !deviceLoopback && devInfo.name != nullptr)
            return true;
    }

    return false;
}

bool Record::StartRecording() noexcept
{
    if(!Record::initStatus || Record::recordStatus || Record::checkStatus)
        return false;

    if(!PluginConfig::GetMicroEnable())
        return false;

    LogVoice("[sv:dbg:record:startrecording] : channel recording starting...");

    BASS_ChannelPlay(Record::recordChannel, 0);
    Record::recordStatus = true;

    return true;
}

bool Record::IsRecording() noexcept
{
    return Record::recordStatus;
}

void Record::StopRecording() noexcept
{
    if(!Record::initStatus)
        return;

    Record::recordStatus = false;

    if(Record::checkStatus)
        return;

    BASS_ChannelPause(Record::recordChannel);
    opus_encoder_ctl(Record::encoder, OPUS_RESET_STATE);

    LogVoice("[sv:dbg:record:stoprecording] : channel recording stoped");

    const auto bufferSize = BASS_ChannelGetData(Record::recordChannel, nullptr, BASS_DATA_AVAILABLE);
    if(bufferSize == -1 && bufferSize == 0) return;

    BASS_ChannelGetData(Record::recordChannel, nullptr, bufferSize);
}

bool Record::StartChecking() noexcept
{
    if(!Record::initStatus || Record::checkStatus)
        return false;

    if(!PluginConfig::GetMicroEnable())
        return false;

    Record::StopRecording();

    LogVoice("[sv:dbg:record:startchecking] : checking device starting...");

    BASS_ChannelPlay(Record::checkChannel, 1);
    BASS_ChannelPlay(Record::recordChannel, 1);
    Record::checkStatus = true;

    return true;
}

bool Record::IsChecking() noexcept
{
    return Record::checkStatus;
}

void Record::StopChecking() noexcept
{
    if(Record::initStatus && Record::checkStatus)
    {
        LogVoice("[sv:dbg:record:stopchecking] : checking device stoped");

        BASS_ChannelStop(Record::checkChannel);
        Record::checkStatus = false;
    }
}

bool Record::GetMicroEnable() noexcept
{
    return PluginConfig::GetMicroEnable();
}

int Record::GetMicroVolume() noexcept
{
    return PluginConfig::GetMicroVolume();
}

int Record::GetMicroDevice() noexcept
{
    return Record::usedDeviceIndex;
}

void Record::SetMicroEnable(const bool microEnable) noexcept
{
    if(!Record::initStatus)
        return;

    PluginConfig::SetMicroEnable(microEnable);

    if(!PluginConfig::GetMicroEnable())
    {
        Record::StopRecording();
        Record::StopChecking();
    }
}

void Record::SetMicroVolume(const int microVolume) noexcept
{
    if(!Record::initStatus)
        return;

    PluginConfig::SetMicroVolume(clampEx(microVolume, 0, 100));

    BASS_RecordSetInput(-1, BASS_INPUT_ON, static_cast<float>
        (PluginConfig::GetMicroVolume()) / 100.f);
}

void Record::SetMicroDevice(const int deviceIndex) noexcept
{
    if(!Record::initStatus)
        return;

    /*const auto devNamesCount = Record::deviceNamesList.size();
    const auto devNumbersCount = Record::deviceNumbersList.size();

    if(deviceIndex < 0 || deviceIndex >= fmin(devNamesCount, devNumbersCount))
        return;

    if(deviceIndex == Record::usedDeviceIndex)
        return;

    BASS_ChannelStop(Record::recordChannel);
    BASS_RecordFree();

    const auto oldDevIndex = Record::usedDeviceIndex;

    if(BASS_RecordInit(Record::deviceNumbersList[Record::usedDeviceIndex = deviceIndex]) == 0 &&
        BASS_RecordInit(Record::deviceNumbersList[Record::usedDeviceIndex = oldDevIndex]) == 0)
        return;

    Record::recordChannel = BASS_RecordStart(SV::kFrequency2, 1, !Record::recordStatus &&
        !Record::checkStatus ? BASS_RECORD_PAUSE : NULL, nullptr, nullptr);

    PluginConfig::SetDeviceName(Record::deviceNamesList[Record::usedDeviceIndex]);*/
}

void Record::SyncConfigs() noexcept
{
    Record::SetMicroEnable(PluginConfig::GetMicroEnable());
    Record::SetMicroVolume(PluginConfig::GetMicroVolume());
}

void Record::ResetConfigs() noexcept
{
    PluginConfig::SetMicroEnable(PluginConfig::kDefValMicroEnable);
    PluginConfig::SetMicroVolume(PluginConfig::kDefValMicroVolume);
}

const std::vector<std::string>& Record::GetDeviceNamesList() noexcept
{
    return Record::deviceNamesList;
}

const std::vector<int>& Record::GetDeviceNumbersList() noexcept
{
    return Record::deviceNumbersList;
}

bool Record::initStatus { false };

bool Record::checkStatus { false };
bool Record::recordStatus { false };

HRECORD Record::recordChannel { NULL };
OpusEncoder* Record::encoder { nullptr };

std::array<opus_int16, SV::kFrameSizeInSamples> Record::encBuffer {};

HSTREAM Record::checkChannel { NULL };

int Record::usedDeviceIndex { -1 };
std::vector<std::string> Record::deviceNamesList;
std::vector<int> Record::deviceNumbersList;

SpeexEchoState *Record::speexEchoState { NULL };
SpeexPreprocessState *Record::speexPreprocessState { NULL };