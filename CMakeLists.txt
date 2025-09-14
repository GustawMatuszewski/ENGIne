cmake_minimum_required(VERSION 3.10)

project(ENGIne CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(OpenGL_GL_PREFERENCE GLVND)

find_package(glfw3 3.3 REQUIRED)
find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)
find_package(assimp REQUIRED)

add_executable(
    ENGIne src/main.cpp
    src/Mesh.cpp
    src/Shader.cpp
    src/Window.cpp
    src/Camera.cpp
    src/Texture.cpp
    src/Light.cpp
    src/Material.cpp
    src/DirectionalLight.cpp
    src/PointLight.cpp
    src/SpotLight.cpp
    src/Model.cpp
)

target_include_directories(ENGIne PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/headers
)

target_link_libraries(ENGIne PRIVATE glfw OpenGL::GL GLEW::GLEW assimp::assimp)

install(TARGETS ENGIne DESTINATION bin)
