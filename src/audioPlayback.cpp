/*
 *  @name: audioPlayback.cpp
 *
 *  @author: Shauna Recto
 *
 *  @description: This source file contains the implementations of the Audio
 *      Playback class.
 *
 */

#include "audioPlayback.h"
#include <fmt/core.h>

ADAudioPlayback::ADAudioPlayback(std::string filename)
{
    fmt::print("Opening Audio file {}...\n", filename);
    result = ma_decoder_init_file(filename.c_str(), NULL, &decoder);
    if (result != MA_SUCCESS) {
        fmt::print(stderr, "File opened does not exist or is not supported...\n");
    }

    deviceConfig = ma_device_config_init(ma_device_type_playback);
    deviceConfig.playback.format = decoder.outputFormat;
    deviceConfig.playback.channels = decoder.outputChannels;
    deviceConfig.sampleRate = decoder.outputSampleRate;
    deviceConfig.dataCallback = &dataCallback;
    deviceConfig.pUserData = &decoder;
}

ADAudioPlayback::~ADAudioPlayback()
{
    ma_device_uninit(&device);
    ma_decoder_uninit(&decoder);
}

void ADAudioPlayback::playAudio()
{
    if (ma_device_init(NULL, &deviceConfig, &device) != MA_SUCCESS) {
        fmt::print(stderr, "Failed to open playback device.\n");
        return;
    }

    if (ma_device_start(&device) != MA_SUCCESS) {
        fmt::print(stderr, "Failed to start playback device.\n");
        return;
    }
}

void ADAudioPlayback::dataCallback(ma_device* pDev, void* pOut, const void* pIn, ma_uint32 frameCount)
{

    ma_decoder* pDec = static_cast<ma_decoder*>(pDev->pUserData);
    if (pDec == nullptr) {
        return;
    }

    // fmt::print("Number of read frames: {} ...\n", frameCount);
    ma_decoder_read_pcm_frames(pDec, pOut, frameCount, nullptr);
    static_cast<void>(pIn);
}