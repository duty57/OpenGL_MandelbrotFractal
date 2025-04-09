cmake_minimum_required(VERSION 3.29)
project(OpenGL_MandelbrotFractal)

set(CMAKE_CXX_STANDARD 20)

add_executable(OpenGL_MandelbrotFractal src/main.cpp Dep/glad/src/glad.c
        src/Shader.cpp
        include/Shader.h
        src/Input.cpp
        include/Input.h
        src/main.cpp)

set(PROJECT_DIR "C:/Users/Eugene/CLionProjects/OpenGL_MandelbrotFractal")

set(GLAD_INCLUDE_DIR "${CMAKE_SOURCE_DIR}/Dep/glad/include")
set(GLFW_INCLUDE_DIR "${CMAKE_SOURCE_DIR}/Dep/GLFW/include")
set(GLFW_LIB_DIR "${CMAKE_SOURCE_DIR}/Dep/GLFW/mingw")
set(GLM_INCLUDE_DIR "${CMAKE_SOURCE_DIR}/Dep/glm")
set(HEADER_INCLUDE_DIR "${CMAKE_SOURCE_DIR}/include")


target_include_directories(OpenGL_MandelbrotFractal PRIVATE  ${GLAD_INCLUDE_DIR} ${GLFW_INCLUDE_DIR} ${GLM_INCLUDE_DIR} ${HEADER_INCLUDE_DIR})

find_library(GLFW_LIB glfw3 PATHS ${GLFW_LIB_DIR} NO_DEFAULT_PATH)

target_link_libraries(OpenGL_MandelbrotFractal ${GLFW_LIB})

find_package(OpenGL REQUIRED)
target_link_libraries(OpenGL_MandelbrotFractal OpenGL::GL)

add_compile_definitions(PROJECT_PATH="${PROJECT_DIR}")
