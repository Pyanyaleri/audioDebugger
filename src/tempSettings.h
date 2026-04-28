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
extern inline float tempConf_WhistleBias = 1.3;
extern inline float tempConf_WhistleConfidenceThresh = 0.25;
extern inline float tempConf_UpperWhistleConfidenceThresh = 0.70;
extern inline int tempConf_AveragingLen = 3;

extern inline int tempConf_FFTSize = 256;
extern inline int tempConf_FFTStep = 128;
extern inline int tempConf_NumFFTs = 15;

extern inline float tempConf_AudioGain = 2.7;
extern inline int tempConf_SamplingRate = 44100;

#endif /* TEMP_SETTINGS_H */