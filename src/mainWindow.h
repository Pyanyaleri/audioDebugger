/*
 *  @name: mainWindow.h
 *
 *  @author: Shauna Recto
 *
 *  @description: This header contains all the necessary member declarations
 *      and default initializaitons for the main window of the program.
 *
 */

#include "imgui.h"

#include <string>

#ifndef AD_MAIN_WINDOW_H
#define AD_MAIN_WINDOW_H

class ADMainWindow {
private:
    bool isOpen = true;
    ImGuiWindowFlags windowFlags;

    bool bShowAudioTimeline = true;
    bool bShowOscilloscope = true;
    bool bShowSpectrogram = true;
    bool bShowTopMenu = true;
    bool bShowVideo = true;
    bool bShowNNOutput = true;

public:
    static inline std::string PROGRAM_NAME = "RoboÉireann audioDebugger";

    ADMainWindow();
    ~ADMainWindow();

    void update();
    void showMenu();
    void showOscilloscope();
    int parseSettings();

    struct ScrollingBuffer {
        int MaxSize;
        int Offset;
        ImVector<ImVec2> Data;
        ScrollingBuffer(int max_size = 2000)
        {
            MaxSize = max_size;
            Offset = 0;
            Data.reserve(MaxSize);
        }
        void AddPoint(float x, float y)
        {
            if (Data.size() < MaxSize)
                Data.push_back(ImVec2(x, y));
            else {
                Data[Offset] = ImVec2(x, y);
                Offset = (Offset + 1) % MaxSize;
            }
        }
        void Erase()
        {
            if (Data.size() > 0) {
                Data.shrink(0);
                Offset = 0;
            }
        }
    };
};
#endif /* AD_MAIN_WINDOW_H */