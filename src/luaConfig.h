/*
 *  @name: luaConfig.h
 *
 *  @author: Shauna Recto
 *
 *  @description: Header file for using Lua for config files.
 *
 */

#ifndef LUA_CONFIG_H
#define LUA_CONFIG_H

extern "C" {
#include <lauxlib.h>
#include <lua.h>
#include <lualib.h>
}

#include <string>

class LuaConfig {
  private:
	lua_State *state;
	std::string filename;

  public:
	LuaConfig(std::string);
	~LuaConfig();

	bool loadConfigFile();
};

struct ProgramSettings {
	std::string program_name;
	std::string font;
	float font_size;

	std::string test_audio_filename;
	std::string audio_filename;

	int window_w;
	int window_h;

	float audio_gain;
	int sampling_rate;

	int detection_delay_ms;
	float whistle_confidence_thresh;
	int buffer_size;
	int fft_size;
	int fft_step;
};

inline ProgramSettings programSettings;

#endif /* LUA_CONFIG_H */