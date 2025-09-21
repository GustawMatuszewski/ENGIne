cmake_minimum_required(VERSION 3.10)

project(ENGIne CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(OpenGL_GL_PREFERENCE GLVND)

find_package(glfw3 3.3 REQUIRED)
find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)
find_package(assimp REQUIRED)

set(IMGUI_DIR ${CMAKE_SOURCE_DIR}/third_party/imgui)

set(IMGUI_SRC
    ${IMGUI_DIR}/imgui.cpp
    ${IMGUI_DIR}/imgui_demo.cpp
    ${IMGUI_DIR}/imgui_draw.cpp
    ${IMGUI_DIR}/imgui_tables.cpp
    ${IMGUI_DIR}/imgui_widgets.cpp
    ${IMGUI_DIR}/backends/imgui_impl_glfw.cpp
    ${IMGUI_DIR}/backends/imgui_impl_opengl3.cpp
)

add_executable(
    ENGIne
    src/main.cpp
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
    src/UI.cpp
    ${IMGUI_SRC}
)

target_include_directories(ENGIne PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/headers
    ${IMGUI_DIR}
    ${IMGUI_DIR}/backends
)

target_link_libraries(ENGIne PRIVATE glfw OpenGL::GL GLEW::GLEW assimp::assimp)

install(TARGETS ENGIne DESTINATION bin)
