#pragma once
#include "Scene.h"

class DefaultScene : public Scene {
public:
    DefaultScene();
    void Update() override;
    ~DefaultScene() override;
};
