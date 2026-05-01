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

extern "C" {
#include <fftw3.h>
#include <lauxlib.h>
#include <lua.h>
#include <lualib.h>
}

/* Standard Library inclusions */
#include <cstdlib>
#include <string>

/* Local Library inclusions */
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#define IMGUI_ENABLE_FREETYPE

#include "implot.h"
#include "implot_internal.h"

#include "miniaudio.h"

/* stb image loading inclusion */
#define _CRT_SECURE_NO_WARNINGS
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include "RE2023_whistleDetector.h"
#include "adFFT.h"
#include "audioPlayback.h"
#include "audioRW.h"
#include "luaConfig.h"
#include "mainWindow.h"

static void glfw_error_callback(int, const char*);
void customTestWindow(ImGuiIO*, bool&, bool&, ImVec4*);
bool LoadTextureFromFile(const char*, GLuint*, int*, int*);
bool LoadTextureFromMemory(const void*, size_t, GLuint*, int*, int*);

int main(int, char**)
{
    /* Initializing Lua enviroment */
    LuaConfig luaConfigInstance("settings.lua");
    luaConfigInstance.loadConfigFile();

    glfwSetErrorCallback(glfw_error_callback);
    if (!glfwInit()) {
        return EXIT_FAILURE;
    }

    /* GL 3.0 + GLSL 130 */
    const char* glsl_version = "#version 130";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);

    /* Create window with graphics context */
    GLFWwindow* window = glfwCreateWindow(programSettings.window_w, programSettings.window_h, programSettings.program_name.c_str(), nullptr, nullptr);
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
    ImFont* font = io.Fonts->AddFontFromFileTTF(programSettings.font.c_str(), programSettings.font_size);
    IM_ASSERT(font != nullptr);

    /* Test image loading */
    int my_image_width = 0;
    int my_image_height = 0;
    GLuint my_image_texture = 0;
    bool ret = LoadTextureFromFile("assets/test.jpg", &my_image_texture, &my_image_width, &my_image_height);
    IM_ASSERT(ret);

    /* Our state */
    bool show_demo_window = true;
    ImVec4 clear_color = ImVec4(0.063f, 0.082f, 0.102f, 1.0f);

    fmt::print("Opening windows...\n");
    ADMainWindow mainWindowOBj;

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
            ImPlot::ShowDemoWindow(&show_demo_window);
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

bool LoadTextureFromMemory(const void* data, size_t data_size, GLuint* out_texture, int* out_width, int* out_height)
{
    // Load from file
    int image_width = 0;
    int image_height = 0;
    unsigned char* image_data = stbi_load_from_memory((const unsigned char*)data, (int)data_size, &image_width, &image_height, NULL, 4);
    if (image_data == NULL)
        return false;

    // Create a OpenGL texture identifier
    GLuint image_texture;
    glGenTextures(1, &image_texture);
    glBindTexture(GL_TEXTURE_2D, image_texture);

    // Setup filtering parameters for display
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // Upload pixels into texture
    glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image_width, image_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image_data);
    stbi_image_free(image_data);

    *out_texture = image_texture;
    *out_width = image_width;
    *out_height = image_height;

    return true;
}

// Open and read a file, then forward to LoadTextureFromMemory()
bool LoadTextureFromFile(const char* file_name, GLuint* out_texture, int* out_width, int* out_height)
{
    FILE* f = fopen(file_name, "rb");
    if (f == NULL)
        return false;
    fseek(f, 0, SEEK_END);
    size_t file_size = (size_t)ftell(f);
    if (file_size == -1)
        return false;
    fseek(f, 0, SEEK_SET);
    void* file_data = IM_ALLOC(file_size);
    fread(file_data, 1, file_size, f);
    fclose(f);
    bool ret = LoadTextureFromMemory(file_data, file_size, out_texture, out_width, out_height);
    IM_FREE(file_data);
    return ret;
}