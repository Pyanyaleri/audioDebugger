/*
 *  @name: glfwConfig.h
 *
 *  @author: Shauna Recto
 *
 *  @description: Header file for getting the GLFW infrastructure set up.
 *
 */

#ifndef GLFW_CONFIG_H
#define GLFW_CONFIG_H

#define GL_SILENCE_DEPRECATION
#include <GLFW/glfw3.h> /* Will drag system OpenGL headers */

class GLFW_Config {

private:
    GLFWwindow* window;
    const char* glsl_version;

public:
    GLFW_Config();
    ~GLFW_Config();

    GLFWwindow* getWindow();
    const char* getGLSLVersion();

    static void glfw_error_callback(int, const char*);

    bool windowShouldClose();
};

#endif