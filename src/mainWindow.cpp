/*
 *  @name: mainWindow.cpp
 *
 *  @author: Shauna Recto
 *
 *  @description: This source file contains all the necessaru implementations
 *      of the main window, and all the different GUI functionalities.
 *
 *      It also (should TODO) contains all the boilerplate and the GLFW and
 *      ImGui initializations for the windows.
 *
 */

#include "mainWindow.h"

#include "imgui.h"
#include "implot.h"
#include "implot_internal.h"
#include "tempSettings.h"

#include <fmt/core.h>

#include <algorithm>
#include <cmath>
#include <vector>

ADMainWindow::ADMainWindow()
{
    windowFlags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoBackground;
    childFlags = ImGuiChildFlags_Borders | ImGuiChildFlags_AlwaysAutoResize | ImGuiChildFlags_AutoResizeX | ImGuiChildFlags_AutoResizeY;

    /* Set the default viewport info */
    const ImGuiViewport* viewport = ImGui::GetMainViewport();
    ImGui::SetNextWindowPos(viewport->Pos);
    ImGui::SetNextWindowSize(viewport->Size);

    /* Enabling all widgets */
    at_instance.enable = true;
    ao_instance.enable = true;
    camera_instance.enable = true;
    config_instance.enable = true;
    el_instance.enable = true;
    mo_instance.enable = true;
    spect_instance.enable = true;

    fmt::print("Generating test audio...\n");
    at_instance.loadTestAudio();
}

ADMainWindow::~ADMainWindow() { }

void ADMainWindow::update()
{
    /* Resize the window according to program window size */
    ImGuiIO& windowIO = ImGui::GetIO();
    ImGui::SetNextWindowPos(ImVec2(0, 0));
    ImGui::SetNextWindowSize(windowIO.DisplaySize);

    /* Enable the window widgets */
    ImGui::Begin(PROGRAM_NAME.c_str(), &isOpen, windowFlags);

    /* Obtain the program size */
    currentSize = ImGui::GetContentRegionAvail();

    /* Collates the enables for the top menu */
    bool* enables[] = {
        &at_instance.enable,
        &ao_instance.enable,
        &camera_instance.enable,
        &config_instance.enable,
        &el_instance.enable,
        &mo_instance.enable,
        &spect_instance.enable
    };

    tm_instance.show(enables);

    childWBase = currentSize.x;
    childHBase = currentSize.y / 5.f;

    /* Row 1 */ {
        at_instance.show(childWBase, childHBase, childFlags);
    }

    childWBase = (currentSize.x / 3.f) - 5;
    childHBase = (2 * currentSize.y / 5.f) - 4;

    /* Row 2 */ {
        el_instance.show(childWBase, childHBase, childFlags);
        ImGui::SameLine();
        camera_instance.show(childWBase, childHBase, childFlags);
        ImGui::SameLine();
        config_instance.show(childWBase, childHBase, childFlags);
    }

    /* Row 3 */ {
        ao_instance.show(childWBase, childHBase, childFlags);
        ImGui::SameLine();
        spect_instance.show(childWBase, childHBase, childFlags);
        ImGui::SameLine();
        mo_instance.show(childWBase, childHBase, childFlags);
    }

    ImGui::End();
}

void ADMainWindow::TopMenu::show(bool* enables[])
{
    if (enable) {
        if (ImGui::BeginMenuBar()) {
            if (ImGui::BeginMenu("File")) {
                ImGui::MenuItem("Open Audio File", nullptr, nullptr);
                ImGui::MenuItem("Open Video File", nullptr, nullptr);
                ImGui::EndMenu();
            }

            if (ImGui::BeginMenu("View")) {
                ImGui::SeparatorText("Audio Viewers");
                ImGui::MenuItem("Audio Timeline", NULL, enables[AUDIO_TIMELINE]);
                ImGui::MenuItem("Oscilloscope", NULL, enables[AUDIO_OSCILLOSCOPE]);
                ImGui::MenuItem("Spectrogram", NULL, enables[SPECTROGRAM]);

                ImGui::SeparatorText("Other Tools");
                ImGui::MenuItem("Camera Output", NULL, enables[CAMERA_OUTPUT]);
                ImGui::MenuItem("Model Output", NULL, enables[MODEL_OUTPUT]);
                ImGui::EndMenu();
            }
            ImGui::EndMenuBar();
        }
    }
}

void ADMainWindow::AudioTimeline::show(float sizeW, float sizeH, ImGuiChildFlags flags)
{
    if (enable) {
        ImGui::BeginChild("audio_timeline", ImVec2(sizeW, sizeH), flags);
        ImGui::SeparatorText("Audio Timeline");

        ImPlotFlags plotFlags = ImPlotFlags_NoMenus | ImPlotFlags_NoBoxSelect | ImPlotFlags_NoInputs;
        if (ImPlot::BeginPlot("##plot_timeline", ImGui::GetContentRegionAvail(), plotFlags)) {
            ImPlot::SetupAxisLimits(ImAxis_X1, 0, 30);
            ImPlot::SetupAxisLimits(ImAxis_Y1, -1, 1);
            ImPlot::PlotShaded("##audio_coords", plotTimes.data(), plotMins.data(), plotMaxs.data(), plotTimes.size());
        }

        ImPlot::EndPlot();
        ImGui::EndChild();
    }
}

void ADMainWindow::AudioTimeline::loadTestAudio()
{
    if (enable) {
        int SAMPLE_RATE = 16000;
        int SAMPLE_LENGTH = 30; /* In seconds */
        int TOTAL_SAMPLES = 2000;

        float timestampLength = (SAMPLE_LENGTH * 1.f) / SAMPLE_RATE;
        int totalAudioSamples = SAMPLE_RATE * SAMPLE_LENGTH;

        for (int timelineInd = 0; timelineInd < totalAudioSamples; timelineInd++) {
            float tCoord = timestampLength * timelineInd;
            timelineData.push_back(ImVec2(tCoord, (sinf(tCoord) + sinf(2 * tCoord)) / 2));
        }

        int plottingRatio = static_cast<int>(totalAudioSamples / TOTAL_SAMPLES);

        for (int plottingStep = 0; plottingStep < totalAudioSamples; plottingStep += plottingRatio) {
            float time = (plottingStep * 1.f) / SAMPLE_RATE;
            ImVec2* chunkStart = timelineData.begin() + plottingStep;
            ImVec2* chunkEnd = chunkStart + plottingRatio;

            float min = (*std::min_element(
                             chunkStart, chunkEnd, [](ImVec2& vec1, ImVec2& vec2) {
                                 return vec1.y < vec2.y;
                             }))
                            .y;
            float max = (*std::max_element(
                             chunkStart, chunkEnd, [](ImVec2& vec1, ImVec2& vec2) {
                                 return vec1.y < vec2.y;
                             }))
                            .y;

            plotTimes.push_back(time);
            plotMins.push_back(min);
            plotMaxs.push_back(max);
        }
    }

    fileLoaded = true;
}

void ADMainWindow::AudioOscilloscope::show(float sizeW, float sizeH, ImGuiChildFlags flags)
{
    if (enable) {
        ImGui::BeginChild("mono_audio", ImVec2(sizeW, sizeH), flags);
        ImGui::SeparatorText("Mono Audio");

        static bool paused = false;
        static ScrollingBuffer dataDigital[3];
        static ScrollingBuffer dataAnalog[2];
        static bool showDigital[3] = { true, false, false };
        static bool showAnalog[2] = { true, false };

        char label[32];
        ImGui::Checkbox("Pause", &paused);
        ImGui::Checkbox("digital_0", &showDigital[0]);
        ImGui::SameLine();
        ImGui::Checkbox("digital_1", &showDigital[1]);
        ImGui::SameLine();
        ImGui::Checkbox("digital_2", &showDigital[2]);
        ImGui::SameLine();
        ImGui::Checkbox("analog_0", &showAnalog[0]);
        ImGui::SameLine();
        ImGui::Checkbox("analog_1", &showAnalog[1]);

        static float t = 0, last_t = 0;
        if (!paused) {
            t += ImGui::GetIO().DeltaTime;
            if (t - last_t >= 0.01f) {
                last_t = t;
                // Digital signal values
                if (showDigital[0])
                    dataDigital[0].AddPoint(t, sinf(2 * t) > 0.45);
                if (showDigital[1])
                    dataDigital[1].AddPoint(t, sinf(2 * t) < 0.45);
                if (showDigital[2])
                    dataDigital[2].AddPoint(t, sinf(50 * t) > 0.5);
                // Analog signal values
                if (showAnalog[0])
                    dataAnalog[0].AddPoint(t, sinf(2 * t));
                if (showAnalog[1])
                    dataAnalog[1].AddPoint(t, cosf(2 * t));
            }
        }
        if (ImPlot::BeginPlot("##plot_mono_audio", ImGui::GetContentRegionAvail())) {
            ImPlot::SetupAxisLimits(ImAxis_X1, t - 10.0, t, paused ? ImGuiCond_Once : ImGuiCond_Always);
            ImPlot::SetupAxisLimits(ImAxis_Y1, -1, 1);
            for (int i = 0; i < 3; ++i) {
                if (showDigital[i] && dataDigital[i].Data.size() > 0) {
                    snprintf(label, sizeof(label), "digital_%d", i);
                    ImPlot::PlotDigital(label, &dataDigital[i].Data[0].x, &dataDigital[i].Data[0].y, dataDigital[i].Data.size(), { ImPlotProp_Offset, dataDigital[i].Offset, ImPlotProp_Stride, 2 * sizeof(float), ImPlotProp_Size, (i + 1) * 4 });
                }
            }
            for (int i = 0; i < 2; ++i) {
                if (showAnalog[i]) {
                    snprintf(label, sizeof(label), "analog_%d", i);
                    if (dataAnalog[i].Data.size() > 0) {
                        ImPlot::PlotLine(label, &dataAnalog[i].Data[0].x, &dataAnalog[i].Data[0].y, dataAnalog[i].Data.size(), { ImPlotProp_Offset, dataAnalog[i].Offset, ImPlotProp_Stride, 2 * sizeof(float) });
                    }
                }
            }
            ImPlot::EndPlot();
        }

        ImGui::EndChild();
    }
}

void ADMainWindow::CameraOutput::show(float sizeW, float sizeH, ImGuiChildFlags flags)
{
    if (enable) {
        ImGui::BeginChild("camera_output", ImVec2(sizeW, sizeH), flags);
        ImGui::SeparatorText("Camera");
        ImGui::EndChild();
    }
}

void ADMainWindow::Configuration::show(float sizeW, float sizeH, ImGuiChildFlags flags)
{
    if (enable) {
        ImGui::BeginChild("config", ImVec2(sizeW, sizeH), flags);
        ImGui::SeparatorText("Configuration");
        ImGui::PushItemWidth(100.f);

        ImGui::SeparatorText("Audio File Information");
        ImGui::InputInt("Sampling Rate (Hz)", &tempConf_SamplingRate);

        ImGui::SeparatorText("Audio Manipulation");
        ImGui::InputFloat("Audio Gain", &tempConf_AudioGain);

        ImGui::SeparatorText("Spectrogram Information");
        ImGui::InputInt("FFT Size", &tempConf_FFTSize);
        ImGui::InputInt("FFT Step", &tempConf_FFTStep);
        ImGui::InputInt("Number of FFTs", &tempConf_NumFFTs);

        ImGui::SeparatorText("Whistle Detector Information");
        ImGui::InputInt("Detection Delay (ms)", &tempConf_DetTimeoutMs);
        ImGui::InputFloat("Whistle Confidence Threshold", &tempConf_WhistleConfidenceThresh);

        ImGui::EndChild();
    }
}

void ADMainWindow::EventLog::show(float sizeW, float sizeH, ImGuiChildFlags flags)
{
    if (enable) {
        ImGui::BeginChild("event_log", ImVec2(sizeW, sizeH), flags);
        ImGui::SeparatorText("Event Log");

        const std::vector<std::string> tempList = { "01234: Whistle has been detected", "05678: Whistle has been detected", "01234: Whistle has been detected", "05678: Whistle has been detected" };
        static int list_selected_index = 0;
        int item_highlighted_idx = -1; // Here we store our highlighted data as an index.

        if (ImGui::BeginListBox("##event_log_list_box", ImGui::GetContentRegionAvail())) {
            for (int item = 0; item < static_cast<int>(tempList.size()); item++) {
                bool is_selected = (list_selected_index == item);
                ImGuiSelectableFlags flags = (item_highlighted_idx == item) ? ImGuiSelectableFlags_Highlight : 0;
                if (ImGui::Selectable(tempList[item].c_str(), is_selected, flags))
                    list_selected_index = item;

                if (is_selected)
                    ImGui::SetItemDefaultFocus();
            }
            ImGui::EndListBox();
        }

        ImGui::EndChild();
    }
}

void ADMainWindow::ModelOutput::show(float sizeW, float sizeH, ImGuiChildFlags flags)
{
    if (enable) {
        ImGui::BeginChild("model_output", ImVec2(sizeW, sizeH), flags);
        ImGui::SeparatorText("Model Output");

        static bool paused = false;
        static ScrollingBuffer dataDigital[3];
        static ScrollingBuffer dataAnalog[2];
        static bool showDigital[3] = { true, false, false };
        static bool showAnalog[2] = { true, false };

        char label[32];
        ImGui::Checkbox("Pause", &paused);
        ImGui::Checkbox("digital_0", &showDigital[0]);
        ImGui::SameLine();
        ImGui::Checkbox("digital_1", &showDigital[1]);
        ImGui::SameLine();
        ImGui::Checkbox("digital_2", &showDigital[2]);
        ImGui::SameLine();
        ImGui::Checkbox("analog_0", &showAnalog[0]);
        ImGui::SameLine();
        ImGui::Checkbox("analog_1", &showAnalog[1]);

        static float t = 0, last_t = 0;
        if (!paused) {
            t += ImGui::GetIO().DeltaTime;
            if (t - last_t >= 0.01f) {
                last_t = t;
                // Digital signal values
                if (showDigital[0])
                    dataDigital[0].AddPoint(t, sinf(2 * t) > 0.45);
                if (showDigital[1])
                    dataDigital[1].AddPoint(t, sinf(2 * t) < 0.45);
                if (showDigital[2])
                    dataDigital[2].AddPoint(t, sinf(50 * t) > 0.5);
                // Analog signal values
                if (showAnalog[0])
                    dataAnalog[0].AddPoint(t, sinf(2 * t));
                if (showAnalog[1])
                    dataAnalog[1].AddPoint(t, cosf(2 * t));
            }
        }
        if (ImPlot::BeginPlot("##Digital", ImGui::GetContentRegionAvail())) {
            ImPlot::SetupAxisLimits(ImAxis_X1, t - 10.0, t, paused ? ImGuiCond_Once : ImGuiCond_Always);
            ImPlot::SetupAxisLimits(ImAxis_Y1, -1, 1);
            for (int i = 0; i < 3; ++i) {
                if (showDigital[i] && dataDigital[i].Data.size() > 0) {
                    snprintf(label, sizeof(label), "digital_%d", i);
                    ImPlot::PlotDigital(label, &dataDigital[i].Data[0].x, &dataDigital[i].Data[0].y, dataDigital[i].Data.size(), { ImPlotProp_Offset, dataDigital[i].Offset, ImPlotProp_Stride, 2 * sizeof(float), ImPlotProp_Size, (i + 1) * 4 });
                }
            }
            for (int i = 0; i < 2; ++i) {
                if (showAnalog[i]) {
                    snprintf(label, sizeof(label), "analog_%d", i);
                    if (dataAnalog[i].Data.size() > 0) {
                        ImPlot::PlotLine(label, &dataAnalog[i].Data[0].x, &dataAnalog[i].Data[0].y, dataAnalog[i].Data.size(), { ImPlotProp_Offset, dataAnalog[i].Offset, ImPlotProp_Stride, 2 * sizeof(float) });
                    }
                }
            }
            ImPlot::EndPlot();
        }

        ImGui::EndChild();
    }
}

void ADMainWindow::Spectrogram::show(float sizeW, float sizeH, ImGuiChildFlags flags)
{
    if (enable) {
        ImGui::BeginChild("spectrogram", ImVec2(sizeW, sizeH), flags);
        ImGui::SeparatorText("Spectrogram");
        ImGui::EndChild();
    }
}

void ADMainWindow::ScrollingBuffer::AddPoint(float x, float y)
{
    if (Data.size() < MaxSize)
        Data.push_back(ImVec2(x, y));
    else {
        Data[Offset] = ImVec2(x, y);
        Offset = (Offset + 1) % MaxSize;
    }
}

void ADMainWindow::ScrollingBuffer::Erase()
{
    if (Data.size() > 0) {
        Data.shrink(0);
        Offset = 0;
    }
}