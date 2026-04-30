/*
 *  @name: audioPlayback.h
 *
 *  @author: Shauna Recto
 *
 *  @description: This header file contains the member declarations and default
 *      initializations of the Audio Playback Class.
 *
 *      This current implementation uses the miniaudio library for simplistic
 *      implementation, but can be substituted via other libraries such as
 *      PortAudio? I've not used it before and so can't guarantee it as such.
 *
 */

#ifndef AD_AUDIO_PB_H
#define AD_AUDIO_PB_H

#include "miniaudio.h"
#include <string>

class ADAudioPlayback {
private:
    ma_result result;
    ma_decoder decoder;
    ma_device_config deviceConfig;
    ma_device device;

public:
    ADAudioPlayback();
    ADAudioPlayback(std::string);
    ~ADAudioPlayback();

    static void dataCallback(ma_device*, void*, const void*, ma_uint32);
    void playAudio();
};

#endif /* AD_AUDIO_PB_H */