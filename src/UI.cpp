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

    
    void InspectorPanel(ECSManager& ecsManager, int entityID, float posX, float posY, float width, float height) {
        ImGui::SetNextWindowPos(ImVec2(posX, posY), ImGuiCond_Always);
        ImGui::SetNextWindowSize(ImVec2(width, height), ImGuiCond_Always);

        std::string windowTitle = "Entity " + std::to_string(entityID) + " Inspector";
        ImGui::Begin(windowTitle.c_str());

        TransformController(ecsManager, entityID);
        TextureController(ecsManager, entityID);
        DirectionalLightController(ecsManager, entityID);
        PointLightController(ecsManager, entityID);
        SpotLightController(ecsManager, entityID);
        ImGui::End();
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

    void TransformController(ECSManager& ecsManager, int entityID) {
        if(ecsManager.HasComponent<TransformComponent>(entityID)) {
            TransformComponent& transform = ecsManager.GetTransformComponent(entityID);

            if(ImGui::CollapsingHeader("Transform")) {
                if(ImGui::InputFloat3("Position", &transform.position[0]) ||
                    ImGui::InputFloat3("Rotation", &transform.rotation[0]) ||
                    ImGui::InputFloat3("Scale", &transform.scale[0])) 
                {
                    transform.SetDirty();
                }
            }
        }
    }

    void TextureController(ECSManager& ecsManager, int entityID) {
        if(ecsManager.HasComponent<TextureComponent>(entityID)) {
            TextureComponent& textureComp = ecsManager.GetTextureComponent(entityID);

            if (ImGui::CollapsingHeader("Texture")) {
                if (textureComp.texture) {
                    ImGui::Text("Texture Preview:");
                    GLuint textureID = textureComp.texture->GetTextureID();
                    ImGui::Image((void*)(intptr_t)textureID, ImVec2(100, 100));
                } else {
                    ImGui::Text("No Texture Assigned");
                }
            }
        } else if(ecsManager.HasComponent<ModelComponent>(entityID)) {
            ModelComponent& modelComp = ecsManager.GetModelComponent(entityID);

            if(ImGui::CollapsingHeader("Texture")) {
                if(modelComp.model && !modelComp.model->textureIDs.empty()) {
                    for (size_t i = 0; i < modelComp.model->textureIDs.size(); i++) {
                        GLuint texID = modelComp.model->textureIDs[i];
                        ImGui::Text("Texture %zu:", i);
                        ImGui::Image((void*)(intptr_t)texID, ImVec2(100, 100));
                    }
                } else {
                    ImGui::Text("No Texture Assigned");
                }
            }
        }

    }

    void DirectionalLightController(ECSManager& ecsManager, int entityID) {
        if(!ecsManager.HasComponent<DirectionalLightComponent>(entityID))
            return;

        DirectionalLightComponent& dirLightComp = ecsManager.GetDirectionalLightComponent(entityID);
        DirectionalLight* light = dirLightComp.directionalLight;

        if(!light)
            return;

        if(ImGui::CollapsingHeader("Directional Light")) {

            glm::vec3 color = light->GetColor();
            float ambient = light->GetAmbientIntensity();
            float diffuse = light->GetDiffuseIntensity();
            glm::vec3 direction = light->GetDirection();

            ImGui::ColorEdit3("Color", &color[0]);
            ImGui::SliderFloat("Ambient Intensity", &ambient, 0.0f, 1.0f);
            ImGui::SliderFloat("Diffuse Intensity", &diffuse, 0.0f, 1.0f);
            ImGui::InputFloat3("Direction", &direction[0]);

            light->SetColor(color);
            light->SetAmbientIntensity(ambient);
            light->SetDiffuseIntensity(diffuse);
            light->SetDirection(direction);
        }
    }

    void SpotLightController(ECSManager& ecsManager, int entityID) {
        if (!ecsManager.HasComponent<SpotLightComponent>(entityID))
            return;
        SpotLightComponent& spotLightComp = ecsManager.GetSpotLightComponent(entityID);
        SpotLight* light = spotLightComp.spotLight;
        if (!light)
            return;

        if (ImGui::CollapsingHeader("Spot Light")) {
            glm::vec3 color = light->GetColor();
            glm::vec3 position = light->GetPosition();
            glm::vec3 direction = light->GetDirection();
            GLfloat ambient = light->GetAmbientIntensity();
            GLfloat diffuse = light->GetDiffuseIntensity();
            GLfloat constant = light->GetConstant();
            GLfloat linear = light->GetLinear();
            GLfloat exponent = light->GetExponent();
            GLfloat edge = light->GetEdge();

            ImGui::ColorEdit3("Color", &color[0]);
            ImGui::SliderFloat("Ambient Intensity", &ambient, 0.0f, 1.0f);
            ImGui::SliderFloat("Diffuse Intensity", &diffuse, 0.0f, 1.0f);
            ImGui::SliderFloat("Constant Intensity", &constant, 0.0f, 1.0f);
            ImGui::SliderFloat("Linear Intensity", &linear, 0.0f, 1.0f);
            ImGui::SliderFloat("Exponent Intensity", &exponent, 0.0f, 1.0f);
            ImGui::SliderFloat("Edge", &edge, 0.0f, 1.0f);
            ImGui::InputFloat3("Position", &position[0]);
            ImGui::InputFloat3("Direction", &direction[0]);

            light->SetColor(color);
            light->SetAmbientIntensity(ambient);
            light->SetDiffuseIntensity(diffuse);
            light->SetConstant(constant);
            light->SetLinear(linear);
            light->SetExponent(exponent);
            light->SetEdge(edge);
            light->SetPosition(position);
            light->SetDirection(direction);
        }
    }

    void PointLightController(ECSManager& ecsManager, int entityID){
        if(!ecsManager.HasComponent<PointLightComponent>(entityID))
            return;

        PointLightComponent& pointLightComp = ecsManager.GetPointLightComponent(entityID);
        PointLight* light = pointLightComp.pointLight;

        if(!light)
            return;

        if(ImGui::CollapsingHeader("Point Light")) {
            glm::vec3 color = light->GetColor();
            glm::vec3 position = light->GetPosition();
            float ambient = light->GetAmbientIntensity();
            float diffuse = light->GetDiffuseIntensity();
            float constant = light->GetConstant();
            float linear = light->GetLinear();
            float exponent = light->GetExponent();

            ImGui::ColorEdit3("Color", &color[0]);
            ImGui::SliderFloat("Ambient Intensity", &ambient, 0.0f, 1.0f);
            ImGui::SliderFloat("Diffuse Intensity", &diffuse, 0.0f, 1.0f);
            ImGui::SliderFloat("Constant Intensity", &constant, 0.0f, 1.0f);
            ImGui::SliderFloat("Linear Intensity", &linear, 0.0f, 1.0f);
            ImGui::SliderFloat("Exponent Intensity", &exponent, 0.0f, 1.0f);
            ImGui::InputFloat3("Position", &position[0]);

            light->SetColor(color);
            light->SetAmbientIntensity(ambient);
            light->SetDiffuseIntensity(diffuse);
            light->SetConstant(constant);
            light->SetLinear(linear);
            light->SetExponent(exponent);
            light->SetPosition(position);
        }
        
    }



}

