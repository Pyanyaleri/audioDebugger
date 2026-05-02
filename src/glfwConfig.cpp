/*
 *  @name: glfwConfig.cpp
 *
 *  @author: Shauna Recto
 *
 *  @description: Source file for getting the GLFW infrastructure set up.
 *
 */

#include "glfwConfig.h"
#include "luaConfig.h"
#include <cassert>
#include <fmt/core.h>

GLFW_Config::GLFW_Config() {
	glfwSetErrorCallback(glfw_error_callback);
	assert(glfwInit());

	glsl_version = "#version 130";
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);

	window = glfwCreateWindow(
		programSettings.window_w, programSettings.window_h,
		programSettings.program_name.c_str(), nullptr, nullptr);
	assert(window != nullptr);

	glfwMakeContextCurrent(window);
	glfwSwapInterval(1); /* Enable vsync */
}

GLFW_Config::~GLFW_Config() {
	glfwDestroyWindow(window);
	glfwTerminate();
}

GLFWwindow *GLFW_Config::getWindow() { return window; }

const char *GLFW_Config::getGLSLVersion() { return glsl_version; }

void GLFW_Config::glfw_error_callback(int error, const char *description) {
	fmt::print(stderr, "GLFW Error {}: {}\n", error, description);
}

bool GLFW_Config::windowShouldClose() { return glfwWindowShouldClose(window); }