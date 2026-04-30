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

#include <cmath>
#include <vector>

ADMainWindow::ADMainWindow()
{
    windowFlags = 0;

    if (bShowTopMenu) {
        windowFlags |= ImGuiWindowFlags_MenuBar;
    }
    windowFlags = ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoBackground;

    const ImGuiViewport* viewport = ImGui::GetMainViewport();
    ImGui::SetNextWindowPos(viewport->Pos);
    ImGui::SetNextWindowSize(viewport->Size);
}

ADMainWindow::~ADMainWindow()
{
}

void ADMainWindow::update()
{
    /* Resize the window according to program window size */
    ImGuiIO& windowIO = ImGui::GetIO();
    ImGui::SetNextWindowPos(ImVec2(0, 0));
    ImGui::SetNextWindowSize(windowIO.DisplaySize);

    ImGui::Begin(PROGRAM_NAME.c_str(), &isOpen, windowFlags);
    showMenu();
    showOscilloscope();
    ImGui::End();
}

void ADMainWindow::showMenu()
{

    if (ImGui::BeginMenuBar()) {
        if (ImGui::BeginMenu("File")) {
            ImGui::MenuItem("Open Audio File", nullptr, nullptr);
            ImGui::MenuItem("Open Video File", nullptr, nullptr);
            ImGui::MenuItem("Open Open B-Human Log", nullptr, nullptr);
            ImGui::EndMenu();
        }

        if (ImGui::BeginMenu("View")) {
            ImGui::SeparatorText("Audio Viewers");
            ImGui::MenuItem("Audio Timeline", NULL, &bShowAudioTimeline);
            ImGui::MenuItem("Oscilloscope", NULL, &bShowOscilloscope);
            ImGui::MenuItem("Spectrogram", NULL, &bShowSpectrogram);
            ImGui::SeparatorText("Other Tools");
            ImGui::MenuItem("NN Output", NULL, &bShowNNOutput);
            ImGui::MenuItem("Video", NULL, &bShowVideo);
            ImGui::EndMenu();
        }
        ImGui::EndMenuBar();
    }
}

void ADMainWindow::showOscilloscope()
{

    if (bShowOscilloscope) {

        ImVec2 currentSize = ImGui::GetContentRegionAvail();

        float rowSize = currentSize.y / 5.f;
        float columnSize = currentSize.x;
        ImGuiChildFlags childFlags = ImGuiChildFlags_Borders | ImGuiChildFlags_AlwaysAutoResize | ImGuiChildFlags_AutoResizeX | ImGuiChildFlags_AutoResizeY;

        /* Audio Seeker */ {
            ImGui::BeginChild("audio_seeker", ImVec2(columnSize, rowSize), childFlags);
            ImGui::SeparatorText("Audio Seeker");
            ImGui::EndChild();
        }

        rowSize = (2 * currentSize.y / 5.f) - 4;
        columnSize = (currentSize.x / 3.f) - 5;

        /* Event Log */ {
            ImGui::BeginChild("event_log", ImVec2(columnSize, rowSize), childFlags);
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

        ImGui::SameLine();

        /* Camera Output */ {
            ImGui::BeginChild("camera_output", ImVec2(columnSize, rowSize), childFlags);
            ImGui::SeparatorText("Camera");
            ImGui::EndChild();
        }

        ImGui::SameLine();

        /* Configuration */ {
            ImGui::BeginChild("config", ImVec2(columnSize, rowSize), childFlags);
            ImGui::SeparatorText("Configuration");
            ImGui::PushItemWidth(100.f);

            if (ImGui::TreeNode("Audio")) {
                ImGui::InputFloat("Mono Audio Gain", &tempConf_AudioGain);
                ImGui::InputInt("Sampling Rate (Hz)", &tempConf_SamplingRate);
                ImGui::TreePop();
            }

            if (ImGui::TreeNode("Spectrogram")) {
                ImGui::InputInt("FFT Size", &tempConf_FFTSize);
                ImGui::InputInt("FFT Step", &tempConf_FFTStep);
                ImGui::InputInt("Number of FFTs", &tempConf_NumFFTs);
                ImGui::TreePop();
            }

            if (ImGui::TreeNode("Whistle Detector")) {
                ImGui::InputInt("Detection Timeout (ms)", &tempConf_DetTimeoutMs);
                ImGui::InputFloat("Whistle Bias", &tempConf_WhistleBias);
                ImGui::InputFloat("Lower Whistle Confidence Threshold", &tempConf_WhistleConfidenceThresh);
                ImGui::InputFloat("Upper Whistle Confidence Threshold", &tempConf_UpperWhistleConfidenceThresh);
                ImGui::InputInt("Confidence Averaging Length", &tempConf_AveragingLen);
                ImGui::TreePop();
            }

            ImGui::EndChild();
        }

        /* Mono Audio Oscilloscope */ {
            ImGui::BeginChild("mono_audio", ImVec2(columnSize, rowSize), childFlags);
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

        ImGui::SameLine();

        /* Mono Audio Oscilloscope */ {
            ImGui::BeginChild("spectrogram", ImVec2(columnSize, rowSize), childFlags);
            ImGui::SeparatorText("Spectrogram");
            ImGui::EndChild();
        }

        ImGui::SameLine();

        /* Mono Audio Oscilloscope */ {
            ImGui::BeginChild("model_output", ImVec2(columnSize, rowSize), childFlags);
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
}