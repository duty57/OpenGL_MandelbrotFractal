# OpenGL_MandelbrotFractal
An interactive, real-time Mandelbrot set visualizer built with C++ and OpenGL using compute shaders for efficient rendering. The application supports mouse-controlled zooming, panning, and dynamic rendering based on user input, making it an engaging exploration tool for fractals.

https://github.com/user-attachments/assets/70c36645-15e7-4b9c-aab0-3e8d82acc1bd

## Table of Contents

- [Project Overview](#project-overview)
- [Build Instructions](#build-instructions)
- [Features](#features)
- [Technologies Used](#technologies-used)

## Project Overview

This project demonstrates real-time fractal visualization by rendering the Mandelbrot set on the GPU using **compute shaders**. The application implements smooth, interactive zooming and panning:
- **Mouse-Driven Interactions:** Move the cursor and scroll to navigate and zoom.
- **Auto-Zoom Mode:** Toggle automatic zooming with the `ENTER` key.
- **Efficient Rendering:** Ping-pong double-buffered texture rendering reduces flickering and ensures smooth visual output.
- **Clean Architecture:** Code is modularized using utility classes like `Shader` and `Input` for maintainability and clarity.
- **Interactive Color Mapping:** Change between different color schemes on the fly by pressing keys `1` through `5`.

---

## Build Instructions

This project uses **CMake** for its build configuration. Below is the `CMakeLists.txt` file used to configure and build the project:
> [!NOTE]
> This project has been configured on Windows using the Visual Studio compiler. Adjustments may be required for other platforms.
```cmake
cmake_minimum_required(VERSION 3.29)
project(OpenGL_MandelbrotFractal)

set(CMAKE_CXX_STANDARD 20)

add_executable(OpenGL_MandelbrotFractal 
    src/main.cpp 
    Dep/glad/src/glad.c
    src/Shader.cpp
    include/Shader.h
    src/Input.cpp
    include/Input.h
    src/main.cpp)

# Specify your project directory path
set(PROJECT_DIR "C:/Users/Eugene/CLionProjects/OpenGL_MandelbrotFractal")

# Set include directories for dependencies
set(GLAD_INCLUDE_DIR "${CMAKE_SOURCE_DIR}/Dep/glad/include")
set(GLFW_INCLUDE_DIR "${CMAKE_SOURCE_DIR}/Dep/GLFW/include")
set(GLFW_LIB_DIR "${CMAKE_SOURCE_DIR}/Dep/GLFW/mingw")
set(GLM_INCLUDE_DIR "${CMAKE_SOURCE_DIR}/Dep/glm")
set(HEADER_INCLUDE_DIR "${CMAKE_SOURCE_DIR}/include")

target_include_directories(OpenGL_MandelbrotFractal PRIVATE  ${GLAD_INCLUDE_DIR} ${GLFW_INCLUDE_DIR} ${GLM_INCLUDE_DIR} ${HEADER_INCLUDE_DIR})

# Find and link GLFW
find_library(GLFW_LIB glfw3 PATHS ${GLFW_LIB_DIR} NO_DEFAULT_PATH)
target_link_libraries(OpenGL_MandelbrotFractal ${GLFW_LIB})

# Find and link OpenGL
find_package(OpenGL REQUIRED)
target_link_libraries(OpenGL_MandelbrotFractal OpenGL::GL)

# Define project path macro to use in the code
add_compile_definitions(PROJECT_PATH="${PROJECT_DIR}")
```

> [!IMPORTANT]
> Update the path in `CMakeLists.txt` by modifying:
> 
> ```cmake
> set(PROJECT_DIR "C:/Users/Eugene/CLionProjects/OpenGL_MandelbrotFractal")
> ```
> 
> to match your local repository location.

First of all clone the repo and enter its directory:
```bash
git clone https://github.com/duty57/OpenGL_MandelbrotFractal.git
cd OpenGL_MandelbrotFractal
```

Create a Build Directory:
```bash
mkdir build && cd build
```

Generate the Build System:
```bash
cmake ..
```

Build the Project:
```bash
cmake --build .
```
## Features

Once built and running, the Mandelbrot set visualizer supports the following functionality:

| Feature             | Description |
|---------------------|-------------|
| 🔍 **Zoom In/Out**  | Use the mouse scroll wheel to zoom into or out of the fractal smoothly. |
| 🖱️ **Pan View**     | Move the mouse to update the zoom focus point and explore different regions of the fractal. |
| 🚀 **Auto-Zoom Mode** | Press ENTER to toggle continuous auto-zoom into the fractal. |
| 📈 **Real-time Feedback** | The current frame rate (FPS) is printed in the console for performance evaluation. |
---

## Technologies Used

- **OpenGL 4.3** — Graphics rendering.
- **GLFW** — Window and input management.
- **GLAD** — OpenGL function loading.
- **GLSL Compute Shaders** — Efficient computation for fractal rendering.
- **C++20** — Core language for the project.
- **GLM** — Mathematics library for vector/matrix operations.

---
