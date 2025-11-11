#include "DefaultScene.h"

DefaultScene::DefaultScene() {
    mainWindow = Window(1920, 1080, "Default Scene");
    mainWindow.Initialize();

    mainCamera = Camera(
        glm::vec3(0.0f, 3.0f, 0.0f),
        glm::vec3(0.0f, 1.0f, 0.0f),
        0.0f, 0.0f,
        3.0f, 5.0f
    );

    std::vector<Shader> shaderList;

    auto plane = new Entity();
    eManager.AddComponent(plane, new TransformComponent());
}

void DefaultScene::Update() {

}

DefaultScene::~DefaultScene() = default;
