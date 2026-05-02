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
#include "implot.h"

#include <vector>

#ifndef AD_MAIN_WINDOW_H
#define AD_MAIN_WINDOW_H

class ADMainWindow {
  private:
	bool isOpen = true;
	ImGuiWindowFlags windowFlags;
	ImGuiChildFlags childFlags;

	ImVec2 currentSize;

	enum WIDGET_INDEX {
		AUDIO_TIMELINE = 0,
		AUDIO_OSCILLOSCOPE,
		CAMERA_OUTPUT,
		CONFIGURATION,
		EVENT_LOG,
		MODEL_OUTPUT,
		SPECTROGRAM,
	};

	float childHBase;
	float childWBase;

	class BaseWidget {
	  public:
		bool enable;
		virtual void show(bool *[]) {}
		virtual void show(float, float, ImGuiChildFlags) {}
	};

	class TopMenu : public BaseWidget {
	  public:
		void show(bool *[]);
	};

	class AudioTimeline : public BaseWidget {
	  private:
		ImVector<ImVec2> timelineData;
		std::vector<float> plotTimes, plotMins, plotMaxs;

		ImPlotSpec visualSpec;
		bool fileLoaded = false;

	  public:
		void show(float, float, ImGuiChildFlags) override;
		void loadTestAudio();
	};

	class AudioOscilloscope : public BaseWidget {
	  public:
		void show(float, float, ImGuiChildFlags) override;
	};

	class CameraOutput : public BaseWidget {
	  public:
		void show(float, float, ImGuiChildFlags) override;
	};

	class Configuration : public BaseWidget {
	  public:
		void show(float, float, ImGuiChildFlags) override;
	};

	class EventLog : public BaseWidget {
	  public:
		void show(float, float, ImGuiChildFlags) override;
	};

	class ModelOutput : public BaseWidget {
	  public:
		void show(float, float, ImGuiChildFlags) override;
	};

	class Spectrogram : public BaseWidget {
	  public:
		void show(float, float, ImGuiChildFlags) override;
	};

	struct ScrollingBuffer {
		int MaxSize;
		int Offset;
		ImVector<ImVec2> Data;

		ScrollingBuffer(int max_size = 2000) {
			MaxSize = max_size;
			Offset = 0;
			Data.reserve(MaxSize);
		}

		void AddPoint(float, float);
		void Erase();
	};

	TopMenu tm_instance;
	AudioTimeline at_instance;
	AudioOscilloscope ao_instance;
	CameraOutput camera_instance;
	Configuration config_instance;
	EventLog el_instance;
	ModelOutput mo_instance;
	Spectrogram spect_instance;

  public:
	ADMainWindow();
	~ADMainWindow();

	void update();
	int parseSettings();
};
#endif /* AD_MAIN_WINDOW_H */