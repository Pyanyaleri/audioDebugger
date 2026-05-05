# audioDebugger [![Building (Arch Linux)](https://github.com/Pyanyaleri/audioDebugger/actions/workflows/archlinux_build.yml/badge.svg)](https://github.com/Pyanyaleri/audioDebugger/actions/workflows/archlinux_build.yml)
audioDebugger is a audio analysis tool developed for debugging audio related
code used in RoboCup. If the tool is to be used by other teams, this project
should be considered as a contribution by the RoboÉireann team.

> [!WARNING]
> Please note that that this is a WIP! It is currently not capable of analysis just yet (as of 1/5/2026). It is currently in a phase of feature and backend implementation.

# Motivation
As more and more developments in the RoboCup rules are trying to eliminate dependence on wireless forms of communication to each robot and by the Game Controller, audio is one of the next areas of development by teams, especially through Whistle Detectors and encoded signals through audio transmission. This tool aims to conveniently provide utility in analyzing obtained audio data.

# Requirements & Dependencies
In general, the main distro supported is Arch Linux, but debian based distros are also checked for building.

Included dependencies located in `lib/` are:
- [Dear ImGUI](https://github.com/ocornut/imgui) - For the Graphics UI.
- [ImPlots](https://github.com/epezent/implot) - For expanded plotting data.
- [FFTW3](https://fftw.org/) - FFT library.
- [miniaudio](https://github.com/mackron/miniaudio) - For audio parsing and playback.
- [stb Image Header](https://github.com/nothings/stb/blob/master/stb_image.h) - For image rendering.
- ~~[CompiledNN](https://github.com/bhuman/CompiledNN)~~ - Currently under review as is currently being deprecated.

## Arch Linux
> [!NOTE]
> If there is enough interest and would speed up the process of using the program, I can try and implement an `PKGBUILD` for the AUR.
```bash
# pacman -S base-devel alsa-lib fftw fmt glfw glib2 libportal lua protobuf
```
## Debian / Ubuntu
```
apt update && apt install build-essential libasound2-dev libfftw3-dev libfmt-dev libgio-2.0-dev \
  libglfw3-dev libglib2.0-dev libhdf5-dev libportal-dev liblua5.4-dev libprotobuf-dev protobuf-compiler
```
One thing you also have to run *once* is to change the makefile compile inclusions, so run these:
```bash
sed -i 's/-lhdf5 -llua/$(shell pkg-config --libs lua5.4 hdf5)/' Makefile
sed -i 's/pkg-config --cflags glfw3/pkg-config --cflags glfw3 hdf5 lua5.4/' Makefile
```

# Building
Building is done through the makefile, and so you can execute the following:
```bash
mkdir build
make
```
# Licences
Licenses for all of the libraries, fonts, and audio are located in each of their
directories. For RoboÉireann code adapted for the project, see
[here](https://github.com/roboeireann/RoboEireannCodeRelease/blob/main/Licence.md).

A quick list of all the different types of licenses in this project include:
- BSD (3 Clause)
- MIT License
- GPLv2 License
- OFL License
- CC-BY-NC-SA 3.0
