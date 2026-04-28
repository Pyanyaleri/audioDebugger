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
#include "tempSettings.h"
#include <cmath>
#include <fmt/core.h>
#include <vector>

ADMainWindow::ADMainWindow()
{
    windowFlags = 0;

    if (bShowTopMenu) {
        windowFlags |= ImGuiWindowFlags_MenuBar;
    }
    windowFlags = ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoBringToFrontOnFocus;

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
        ImGuiChildFlags oscFlags = ImGuiChildFlags_Borders | ImGuiChildFlags_AlwaysAutoResize | ImGuiChildFlags_AutoResizeX | ImGuiChildFlags_AutoResizeY;

        /* Audio Seeker */ {
            ImGui::BeginChild("audio_seeker", ImVec2(columnSize, rowSize), oscFlags);
            ImGui::SeparatorText("Audio Seeker");
            ImGui::EndChild();
        }

        rowSize = (2 * currentSize.y / 5.f) - 4;
        columnSize = (currentSize.x / 3.f) - 5;

        /* Event Log */ {
            ImGui::BeginChild("event_log", ImVec2(columnSize, rowSize), oscFlags);
            ImGui::SeparatorText("Event Log");

            const std::vector<std::string> tempList = { "01234: Whistle has been detected", "05678: Whistle has been detected", "01234: Whistle has been detected", "05678: Whistle has been detected" };
            static int list_selected_index = 0;
            int item_highlighted_idx = -1; // Here we store our highlighted data as an index.

            if (ImGui::BeginListBox("##event_log_list_box", ImVec2(columnSize - 16, rowSize - 40))) {
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
            ImGui::BeginChild("camera_output", ImVec2(columnSize, rowSize), oscFlags);
            ImGui::SeparatorText("Camera");
            ImGui::EndChild();
        }

        ImGui::SameLine();

        /* Configuration */ {
            ImGui::BeginChild("config", ImVec2(columnSize, rowSize), oscFlags);
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
            ImGui::BeginChild("mono_audio", ImVec2(columnSize, rowSize), oscFlags);
            ImGui::SeparatorText("Mono Audio");
            ImGui::EndChild();
        }

        ImGui::SameLine();

        /* Mono Audio Oscilloscope */ {
            ImGui::BeginChild("spectrogram", ImVec2(columnSize, rowSize), oscFlags);
            ImGui::SeparatorText("Spectrogram");
            ImGui::EndChild();
        }

        ImGui::SameLine();

        /* Mono Audio Oscilloscope */ {
            ImGui::BeginChild("model_output", ImVec2(columnSize, rowSize), oscFlags);
            ImGui::SeparatorText("Model Output");
            ImGui::EndChild();
        }

        // static float arr[] = { 0.6f, 0.1f, 1.0f, 0.5f, 0.92f, 0.1f, 0.2f };
        // ImGui::PlotLines("Frame Times", arr, IM_ARRAYSIZE(arr));

        // // Fill an array of contiguous float values to plot
        // // Tip: If your float aren't contiguous but part of a structure, you can pass a pointer to your first float
        // // and the sizeof() of your structure in the "stride" parameter.
        // static float values[90] = {};
        // static int values_offset = 0;
        // static double refresh_time = 0.0;
        // if (!animate || refresh_time == 0.0)
        //     refresh_time = ImGui::GetTime();
        // while (refresh_time < ImGui::GetTime()) // Create data at fixed 60 Hz rate for the demo
        // {
        //     static float phase = 0.0f;
        //     values[values_offset] = cosf(phase);
        //     values_offset = (values_offset + 1) % IM_ARRAYSIZE(values);
        //     phase += 0.10f * values_offset;
        //     refresh_time += 1.0f / 60.0f;
        // }

        // // Plots can display overlay texts
        // // (in this example, we will display an average value)
        // {
        //     float average = 0.0f;
        //     for (int n = 0; n < IM_ARRAYSIZE(values); n++)
        //         average += values[n];
        //     average /= (float)IM_ARRAYSIZE(values);
        //     char overlay[32];
        //     sprintf(overlay, "avg %f", average);
        //     ImGui::PlotLines("Lines", values, IM_ARRAYSIZE(values), values_offset, overlay, -1.0f, 1.0f, ImVec2(0, 80.0f));
        // }
    }
}