# DH2323 Project
Project for the DH2323 Computer Graphics and Interaction course

## Project blog

The project blog is written as a Markdown file with headings and can be found [here](blog/project_blog.md).

## Contributions

This project is based on the skeleton code from the course labs. Some code is re-used from my own submissions to labs 1 and 2.

The project also uses SDL2 and glm. All code in the `src` directory is written by myself, unless explicitly stated otherwise (most notably, the methods in `Sampling.cpp` were effectively drag-and-dropped). 

The scene in `CornellBox.h` is from the lab assignment, which is a derivative work of the classic "Cornell box" scene developed at Cornell University.

## Build process

Requirements:
- SDL2, specifically SDL-devel-2.32.2-VC.zip (SDL2 version 2.32.2 for Visual Studio)
- CMake version 3.31+
- Visual Studio 2022

Using CMake GUI (cmake-gui.exe), select the location of the source files (this folder).
Create a folder called "project" and select that as the build location.

Click Generate. Select Visual Studio 2022. Expect errors.

Set the SDL2MAIN_LIBRARY, SDL2_DIR, SDL2_INCLUDE_DIR, SDL2_LIBRARY entries as follows:

- SDL2MAIN_LIBRARY (file): SDL2-2.32.2\lib\x64\SDL2.lib
- SDL2_DIR (path): SDL2-2.32.2
- SDL2_INCLUDE_DIR (path): SDL2-2.32.2\include
- (may not be required) SDL2_LIBRARY (file): SDL2-2.32.2\lib\x64\SDL2.lib

Click Generate. Navigate to the project folder and open the .sln file in Visual Studio.
In VS, Build -> Build Solution. Expect errors.

Now navigate to copy the file SDL2-2.32.2\lib\x64\SDL2.dll into project\Debug\SDL2.dll

Again, Build -> Build Solution.

Run project\Debug\DH2323ProjectSDL2.exe file
