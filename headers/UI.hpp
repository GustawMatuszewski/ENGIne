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

#include "EcsManager.hpp"
#include "Entity.hpp"

namespace UI {
    void Init(GLFWwindow* window);

    void InspectorPanel(ECSManager& ecsManager, int entityID, float posX, float posY, float width, float height);

    void FpsCounter(int fps, float posX, float posY, float width, float height);
    void FileExplorer(std::string& startPath);
    void TransformController(ECSManager& ecsManager, int entityID);
    void TextureController(ECSManager& ecsManager, int entityID);

    void GetIo(ImGuiIO& io);
    void Destroy();
}

#endif