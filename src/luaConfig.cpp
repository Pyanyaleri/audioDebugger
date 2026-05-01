/*
 *  @name: luaConfig.cpp
 *
 *  @author: Shauna Recto
 *
 *  @description: Source file for using Lua for config files.
 *
 */

#include "luaConfig.h"
#include <fmt/core.h>
#include <lua.h>

LuaConfig::LuaConfig(std::string config_file)
{
    state = luaL_newstate();
    filename = config_file;

    luaL_openlibs(state);
    luaL_dostring(state, "print('Lua Initialized...')");
}

LuaConfig::~LuaConfig()
{
    lua_close(state);
}

bool LuaConfig::loadConfigFile()
{
    if (luaL_dofile(state, filename.c_str()) != LUA_OK) {

        fmt::print(stderr, "Failed to open {} [returned {}]\n", filename, lua_tostring(state, -1));
        return false;
    }
    fmt::print("Opened {}...\n", filename);

    lua_getglobal(state, "program_name");
    programSettings.program_name = lua_isstring(state, -1) ? lua_tostring(state, -1) : "";
    lua_pop(state, 1);

    lua_getglobal(state, "font");
    programSettings.font = lua_isstring(state, -1) ? lua_tostring(state, -1) : "";
    lua_pop(state, 1);

    lua_getglobal(state, "font_size");
    programSettings.font_size = lua_isnumber(state, -1) ? lua_tonumber(state, -1) : 11;
    lua_pop(state, 1);

    lua_getglobal(state, "test_audio_filename");
    programSettings.test_audio_filename = lua_isstring(state, -1) ? lua_tostring(state, -1) : "";
    lua_pop(state, 1);

    lua_getglobal(state, "audio_filename");
    programSettings.audio_filename = lua_isstring(state, -1) ? lua_tostring(state, -1) : "";
    lua_pop(state, 1);

    lua_getglobal(state, "window_w");
    programSettings.window_w = lua_isinteger(state, -1) ? lua_tointeger(state, -1) : 1280;
    lua_pop(state, 1);

    lua_getglobal(state, "window_h");
    programSettings.window_h = lua_isinteger(state, -1) ? lua_tointeger(state, -1) : 720;
    lua_pop(state, 1);

    lua_getglobal(state, "audio_gain");
    programSettings.audio_gain = lua_isnumber(state, -1) ? lua_tonumber(state, -1) : 1;
    lua_pop(state, 1);

    lua_getglobal(state, "sampling_rate");
    programSettings.sampling_rate = lua_isinteger(state, -1) ? lua_tointeger(state, -1) : 0;
    lua_pop(state, 1);

    lua_getglobal(state, "detection_delay_ms");
    programSettings.detection_delay_ms = lua_isinteger(state, -1) ? lua_tointeger(state, -1) : 0;
    lua_pop(state, 1);

    lua_getglobal(state, "whistle_confidence_thresh");
    programSettings.whistle_confidence_thresh = lua_isnumber(state, -1) ? lua_tonumber(state, -1) : 1;
    lua_pop(state, 1);

    lua_getglobal(state, "buffer_size");
    programSettings.buffer_size = lua_isinteger(state, -1) ? lua_tointeger(state, -1) : 0;
    lua_pop(state, 1);

    lua_getglobal(state, "fft_size");
    programSettings.fft_size = lua_isinteger(state, -1) ? lua_tointeger(state, -1) : 0;
    lua_pop(state, 1);

    lua_getglobal(state, "fft_step");
    programSettings.fft_step = lua_isinteger(state, -1) ? lua_tointeger(state, -1) : 0;
    lua_pop(state, 1);

    return true;
}