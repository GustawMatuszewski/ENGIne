#pragma once

#include "AssetManager.h"
#include "EntityManager.h"
#include "CommonValues.hpp"
#include "Model.hpp"
#include "Shader.hpp"
#include "Window.hpp"
#include "Camera.hpp"
#include "Material.hpp"
#include "Texture.hpp"

class Scene {
    public:
        Scene();
        bool isRunning=false;
        virtual void Update();
        virtual~Scene();

        PointLight pointLights[MAX_POINT_LIGHTS];
        SpotLight spotLights[MAX_SPOT_LIGHTS];

        unsigned int spotLightCount = 0;
        unsigned int pointLightCount = 0;

        EntityManager eManager;
        AssetManager assetManager;

        Window mainWindow;
        Camera mainCamera;
        std::vector<Entity> entityList;

    private:

};
