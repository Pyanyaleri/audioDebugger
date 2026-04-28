/*
 *  @name: RoboÉireann Audio Debugger
 *
 *  @author: Shauna Recto
 *
 *  @description: The main central file of the program that should have most
 *      if not all the top level functions, with most of the code abstracted in
 *      the rest of the headers and sourch files.
 *
 */

/* System Library inclusions */
#define GL_SILENCE_DEPRECATION
#include <GLFW/glfw3.h> /* Will drag system OpenGL headers */

#include <fmt/core.h>

#include <fftw3.h>

/* Standard Library inclusions */
#include <cmath>
#include <cstdlib>
#include <string>

/* Local Library inclusions */
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#define IMGUI_ENABLE_FREETYPE

#include "implot.h"
#include "implot_internal.h"

#define MINIAUDIO_IMPLEMENTATION
#include "miniaudio.h"

#include "RE2023_whistleDetector.h"
#include "adFFT.h"
#include "audioPlayback.h"
#include "audioRW.h"
#include "mainWindow.h"

#include "tempSettings.h"

static void glfw_error_callback(int, const char*);
void customTestWindow(ImGuiIO*, bool&, bool&, ImVec4*);

int main(int, char**)
{

    glfwSetErrorCallback(glfw_error_callback);
    if (!glfwInit()) {
        return EXIT_FAILURE;
    }

    /* GL 3.0 + GLSL 130 */
    const char* glsl_version = "#version 130";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);

    /* Create window with graphics context */
    GLFWwindow* window = glfwCreateWindow(tempConf_WINDOW_W, tempConf_WINDOW_H, tempConf_PROGRAM_NAME.c_str(), nullptr, nullptr);
    if (window == nullptr) {
        return EXIT_FAILURE;
    }

    glfwMakeContextCurrent(window);
    glfwSwapInterval(1); /* Enable vsync */

    /* Setup Dear ImGui context */
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImPlot::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad; // Enable Gamepad Controls

    /* Setup Dear ImGui style */
    ImGui::StyleColorsDark();

    /* Setup Platform/Renderer backends */
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(glsl_version);

    /* Load Fonts */
    ImFont* font = io.Fonts->AddFontFromFileTTF(tempConf_FONT_FNAME.c_str(), tempConf_FONT_SIZE);
    IM_ASSERT(font != nullptr);

    /* Our state */
    bool show_demo_window = true;
    ImVec4 clear_color = ImVec4(0.063f, 0.082f, 0.102f, 1.0f);

    fmt::print("Opening windows...\n");
    ADMainWindow mainWindowOBj;

    // ADAudioPlayback testMusic(tempConf_audioFile.c_str());

    // testMusic.playAudio();

    while (!glfwWindowShouldClose(window)) {

        /* Poll and handle events (inputs, window resize, etc.) */
        glfwPollEvents();

        // Start the Dear ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        /* Demo Window*/
        if (show_demo_window) {
            ImGui::ShowDemoWindow(&show_demo_window);
        }

        /* Main Window */
        mainWindowOBj.update();

        // Rendering
        ImGui::Render();
        int display_w, display_h;
        glfwGetFramebufferSize(window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        glClearColor(clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window);
    }

    /* Cleanup */
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    ImPlot::DestroyContext();

    glfwDestroyWindow(window);
    glfwTerminate();

    fmt::print("Program Completed!\n");
    return EXIT_SUCCESS;
}

static void glfw_error_callback(int error, const char* description)
{
    fmt::print(stderr, "GLFW Error {}: {}\n", error, description);
}