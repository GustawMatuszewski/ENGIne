#include "UI.hpp"

namespace fs = std::filesystem;

namespace UI {
    void Init(GLFWwindow* window) {
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGui::StyleColorsLight();
        ImGui_ImplGlfw_InitForOpenGL(window, true);
        ImGui_ImplOpenGL3_Init("#version 330");
    }

    void GetIo(ImGuiIO& io) {
        io = ImGui::GetIO();
    }

    void Destroy() {
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();
    }

    void FpsCounter(int fps, float posX, float posY, float width, float height) {

        ImGui::SetNextWindowPos(ImVec2(posX, posY), ImGuiCond_Always);   // Set position
        ImGui::SetNextWindowSize(ImVec2(width, height), ImGuiCond_Always); // Set size

        ImGui::Begin("Stats", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove);
        ImGui::Text("FPS: %d", fps);
        ImGui::End();
    }

    void PositionController(std::vector<glm::vec3> &modelPositions, int modelID, float posX, float posY, float width, float height) {

        ImGui::SetNextWindowPos(ImVec2(posX, posY), ImGuiCond_Always);   // Set position
        ImGui::SetNextWindowSize(ImVec2(width, height), ImGuiCond_Always); //set size

        ImGui::Begin("Position Controller");

        if (modelID >= 0 && modelID < modelPositions.size()) {
            std::string label = "Model ID: " + std::to_string(modelID) + " Position";
            ImGui::InputFloat3(label.c_str(), &modelPositions[modelID][0]);
        } else {
            ImGui::Text("Invalid Model ID: %d", modelID);
        }

        ImGui::End();
    }
}