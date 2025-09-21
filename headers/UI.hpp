#ifndef UI_HPP
#define UI_HPP

#include <filesystem>
#include <string>
#include <vector>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "imgui.h"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"

namespace UI {
    void Init(GLFWwindow* window);

    void FpsCounter(int fps, float posX, float posY, float width, float height);
    void FileExplorer(std::string& startPath);
    void PositionController(std::vector<glm::vec3> &modelPositions, int modelID, float posX, float posY, float width, float height);

    void GetIo(ImGuiIO& io);
    void Destroy();
}

#endif