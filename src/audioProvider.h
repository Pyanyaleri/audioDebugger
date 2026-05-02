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

#ifndef AD_AUDIO_H
#define AD_AUDIO_H

#include "miniaudio.h"
#include <string>

class AudioProvider {
  private:
	ma_result result;
	ma_decoder decoder;
	ma_device_config deviceConfig;
	ma_device device;

	bool play;
	bool deviceInitialized;

  public:
	AudioProvider();
	AudioProvider(std::string);
	~AudioProvider();

	static void dataCallback(ma_device *, void *, const void *, ma_uint32);
	void loadAudioFile(std::string);
	void playAudio();
	void pauseAudio();
};

#endif /* AD_AUDIO_H */