EXE = audioDebuggerIMGUI
CXX = g++

# Dir definitions
BUILD_DIR = build
SOURCE_DIR = src
IMGUI_DIR = lib/imgui
IMPLOT_DIR = lib/implot
MINI_AUDIO_DIR = lib/miniaudio
COMP_NN_DIR = lib/CompiledNN

# Project Files
SOURCES =   $(SOURCE_DIR)/main.cpp \
			$(SOURCE_DIR)/mainWindow.cpp \
			$(SOURCE_DIR)/adFFT.cpp \
			$(SOURCE_DIR)/audioPlayback.cpp \
			$(SOURCE_DIR)/audioRW.cpp \
			$(SOURCE_DIR)/RE2023_whistleDetector.cpp \

# IMGUI Dependencies
SOURCES +=  $(IMGUI_DIR)/imgui.cpp \
			$(IMGUI_DIR)/imgui_demo.cpp \
			$(IMGUI_DIR)/imgui_draw.cpp \
			$(IMGUI_DIR)/imgui_tables.cpp \
			$(IMGUI_DIR)/imgui_widgets.cpp \

# IMPLOT Dependencies
SOURCES +=  $(IMPLOT_DIR)/implot_demo.cpp \
			$(IMPLOT_DIR)/implot_items.cpp \
			$(IMPLOT_DIR)/implot.cpp \

# GLFW Dependencies
SOURCES +=  $(IMGUI_DIR)/backends/imgui_impl_glfw.cpp \
			$(IMGUI_DIR)/backends/imgui_impl_opengl3.cpp \

SOURCES +=  $(MINI_AUDIO_DIR)/miniaudio.c \

OBJS = $(addprefix $(BUILD_DIR)/, $(addsuffix .o, $(basename $(notdir $(SOURCES)))))

LINUX_GL_LIBS = -lGL

CXXFLAGS = -std=c++17 \
			 -I$(IMGUI_DIR) \
			 -I$(IMGUI_DIR)/backends \
			 -I$(MINI_AUDIO_DIR) \
			 -I$(IMPLOT_DIR) \
# 			 -I$(COMP_NN_DIR)

CXXFLAGS += -g -Wall -Wformat -Wextra -msse2 -mavx2
LIBS = -lm -lfmt -lpthread -ldl -lfftw3f -lhdf5
LIBS += $(LINUX_GL_LIBS) `pkg-config --static --libs glfw3 protobuf`
LIBS += -L$(COMP_NN_DIR) -l:libCompiledNN.a

CXXFLAGS += `pkg-config --cflags glfw3`
CFLAGS = $(CXXFLAGS)

# For project source files
$(BUILD_DIR)/%.o: $(SOURCE_DIR)/%.cpp
	@printf "Compiling [$<] ...\n"
	@$(CXX) $(CXXFLAGS) -c -o $@ $<

# For IMGUI source files
$(BUILD_DIR)/%.o: $(IMGUI_DIR)/%.cpp
	@printf "Compiling [$<] ...\n"
	@$(CXX) $(CXXFLAGS) -c -o $@ $<

# For IMGUI backend source files
$(BUILD_DIR)/%.o: $(IMGUI_DIR)/backends/%.cpp
	@printf "Compiling [$<] ...\n"
	@$(CXX) $(CXXFLAGS) -c -o $@ $<

# For IMPLOT source files
$(BUILD_DIR)/%.o: $(IMPLOT_DIR)/%.cpp
	@printf "Compiling [$<] ...\n"
	@$(CXX) $(CXXFLAGS) -c -o $@ $<

# For miniaudio source files
$(BUILD_DIR)/%.o: $(MINI_AUDIO_DIR)/%.c
	@printf "Compiling [$<] ...\n"
	@$(CXX) $(CXXFLAGS) -c -o $@ $<

.PHONY: all
all: $(EXE)
	@echo Completed Building For audioDebugger

$(EXE): $(OBJS)
	@printf "Linking [$<] to create $(EXE) ...\n"
	@$(CXX) -o $(BUILD_DIR)/$@ $^ $(CXXFLAGS) $(LIBS)

clean:
	@printf "Deleting all object files and executables...\n"
	@rm -f $(BUILD_DIR)/$(EXE) $(OBJS)
