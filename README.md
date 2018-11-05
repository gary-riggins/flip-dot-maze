# Flip Dot Maze

This is a prototype for a maze navigation to be run on a [Flip Dot Display](https://flipdots.com).

To avoid the time and cost needed to set up flip dot modules, this code simulates the hardware by using a software renderer that can share code used on the hardware controller. All platform-specific code should be contained to files marked accordingly. For instance `main_sdl.cpp` contains all code that depends on SDL, whereas `Keyboard.cpp` has no dependencies beyond the C++ STL.

The goal is to easily test the challenge of developing applications with large, very low resolution, 1-bit display.

<img src="/docs/mazeDemo.gif" alt="Maze Screen Capture"/>

---

## Getting Started

### Prerequisites

* [CMake](https://cmake.org/) - Using Visual Studio 2017 integration
* [SDL2](https://www.libsdl.org) - Developed with version 2.0.4

### Building

The build process runs the included `FindSDL2.cmake` to locate the SDL2 directories. It checks several locations, though it may be simpler to create a `SDL2` environment variable to the directory.

### Running

**Note that while the code should be portable to many platforms, it has only been built on Windows**

The project is currently configured with dynamic linking, so ensure `SDL2.dll` is in the same directory as the executable.

Arrow keys move through the maze.
