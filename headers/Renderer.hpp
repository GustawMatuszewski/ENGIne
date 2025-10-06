#ifndef RENDERER_H
#define RENDERER_H

#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "EcsManager.hpp"
#include "Entity.hpp"
#include "Shader.hpp"
#include "Camera.hpp"
#include "Window.hpp"

class Renderer {
    public:
        Renderer();
        ~Renderer();

        Renderer(Window* window, Camera* camera, ECSManager* ecsManager,
                std::vector<Shader>* shaders,
                DirectionalLight* mainLight,
                PointLight* pointLights, unsigned int* pointLightCount,
                SpotLight* spotLights, unsigned int* spotLightCount,
                glm::mat4* projection);

        void Initialize();
        void Update(float deltaTime);
        void Render();
        void Shutdown();

    private:
        Window* window;
        Camera* camera;
        ECSManager* ecsManager;

        std::vector<Shader>* shaders;
        DirectionalLight* mainLight;
        PointLight* pointLights;
        SpotLight* spotLights;
        unsigned int* pointLightCount;
        unsigned int* spotLightCount;
        glm::mat4* projection;

        GLuint uniformModel;
        GLuint uniformProjection;
        GLuint uniformView;
        GLuint uniformEyePos;
        GLuint uniformSpecularIntensity;
        GLuint uniformShininess;

        void SetupUniforms();
        void DrawEntities();
        void DrawUI();
};

#endif