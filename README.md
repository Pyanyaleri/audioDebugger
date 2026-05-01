# audioDebugger
audioDebugger is a audio analysis tool developed for debugging audio related
code used in RoboCup. If the tool is to be used by other teams, this project
should be considered as a contribution by the RoboÉireann team.

> [!WARNING]
> Please note that that this is a WIP! It is currently not capable of analysis just yet (as of 1/5/2026). It is currently in a phase of feature and backend implementation.

# Motivation
As more and more developments in the RoboCup rules are trying to eliminate dependence on wireless forms of communication to each robot and by the Game Controller, audio is one of the next areas of development by teams, especially through Whistle Detectors and encoded signals through audio transmission. This tool aims to conveniently provide utility in analyzing obtained audio data.

# Requirements & Dependencies
This project relies on several external libraries, some of which have been
precompiled for ease of use. Your mileage may vary if you plan to build on
non-arch based or non-rolling release distributions, however. In this case, a
recompiling is necessary for these libraries. As such, I can try to provide, but
will not have proper means to debug outside of Arch.

The machine I use to compile the project is as follows:
- Arch Linux
- Plasma (Wayland)
- Audio provided by PipeWire (`pipewire, pipewire-pulse, pipewire-jack, wireplumber`)

Included dependencies located in `lib/` are:
- [Dear ImGUI](https://github.com/ocornut/imgui) - For the Graphics UI.
- [ImPlots](https://github.com/epezent/implot) - For expanded plotting data.
- [FFTW3](https://fftw.org/) - FFT library.
- [miniaudio](https://github.com/mackron/miniaudio) - For audio parsing and playback.
- [stb Image Header](https://github.com/nothings/stb/blob/master/stb_image.h) - For image rendering.
- ~~[CompiledNN](https://github.com/bhuman/CompiledNN)~~ Currently under review as is currently being deprecated.

The project also depends on system libraries as these are harder to incorporate
into the system itself. You can install them via Pacman (or any other package
manager but the names will be different):

```bash
# pacman -S base-devel alsa-lib fftw fmt glfw lua protobuf
```

# Building
Building is done through the makefile, and so you can execute the following:
```bash
mkdir build
make
```

If there is enough interest and would speed up the process of using the program,
I can try and implement an `PKGBUILD` for the AUR.

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
