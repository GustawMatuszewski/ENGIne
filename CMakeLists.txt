cmake_minimum_required(VERSION 4.0)

project(ENGIne CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(OpenGL_GL_PREFERENCE GLVND)

# Find dependencies
find_package(glfw3 3.3 REQUIRED)
find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)
find_package(assimp REQUIRED)

# Collect all source files
file(GLOB ENGINE_SOURCES
        src/*.cpp
        Scenes/*.cpp
)

# Create the executable target first
add_executable(ENGIne ${ENGINE_SOURCES}
        src/Scene.cpp
        headers/AssetManager.h
        src/AssetManger.cpp)

# Include directories
target_include_directories(ENGIne PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/headers
        ${CMAKE_CURRENT_SOURCE_DIR}/Scenes
)

# Link libraries
target_link_libraries(ENGIne PRIVATE glfw OpenGL::GL GLEW::GLEW assimp::assimp)

# Install target
install(TARGETS ENGIne DESTINATION bin)
