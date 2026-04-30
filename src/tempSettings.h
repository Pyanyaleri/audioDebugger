/*
 * @name: tempSettings.h
 *
 * @author: Shauna Recto
 *
 * @description: A temporary measure to create a global (dangerous) to store
 *      data for the program to work. There will be some movement to another
 *      config system (most likely lua or cfg systems).
 *
 * @usage: Please make sure that all your configs are prefaced with this style:
 *
 *          extern inline const tempConf_<CONSTANT_NAME>;
 *          extern inline tempConf_<VariableName>;
 */

#ifndef TEMP_SETTINGS_H
#define TEMP_SETTINGS_H

#include <string>

extern inline int tempConf_WINDOW_W = 1600;
extern inline int tempConf_WINDOW_H = 900;
extern inline const std::string tempConf_PROGRAM_NAME = "RoboÉireann audioDebugger";

extern inline const std::string tempConf_FONT_FNAME = "assets/fonts/JosefinSans-Regular.ttf";
extern inline const float tempConf_FONT_SIZE = 14.0f;

extern inline const std::string tempConf_audioFile = "assets/audioSamples/shooting_star.flac";

extern inline int tempConf_DetTimeoutMs = 600;
extern inline float tempConf_WhistleConfidenceThresh = 0.7;

extern inline int tempConf_FFTSize = 256;
extern inline int tempConf_FFTStep = 128;
extern inline int tempConf_NumFFTs = 15;

extern inline float tempConf_AudioGain = 1;
extern inline int tempConf_SamplingRate = 16000;

#endif /* TEMP_SETTINGS_H */

/*
 * whistleDetName = "whistle_model_rc2024.h5";
 f ftSize = 256; *
 fftStep = 128;
 bufferSize = 2048;
 whistleBias = 0;
 whistleConfidenceThresh = 0.65;
 averagingLength = 2;
 sampleGain = 2;
 */