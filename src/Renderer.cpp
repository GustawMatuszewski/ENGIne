#include "Renderer.hpp"

Renderer::Renderer()
    : window(nullptr), camera(nullptr), ecsManager(nullptr),
      shaders(nullptr), mainLight(nullptr),
      pointLights(nullptr), pointLightCount(nullptr),
      spotLights(nullptr), spotLightCount(nullptr),
      projection(nullptr),
      uniformModel(0), uniformProjection(0),
      uniformView(0), uniformEyePos(0),
      uniformSpecularIntensity(0), uniformShininess(0)
{
}

Renderer::~Renderer() {
}

Renderer::Renderer(Window* window, Camera* camera, ECSManager* ecsManager,
                   std::vector<Shader>* shaders,
                   DirectionalLight* mainLight,
                   PointLight* pointLights, unsigned int* pointLightCount,
                   SpotLight* spotLights, unsigned int* spotLightCount,
                   glm::mat4* projection)
    : window(window), camera(camera), ecsManager(ecsManager),
      shaders(shaders), mainLight(mainLight),
      pointLights(pointLights), pointLightCount(pointLightCount),
      spotLights(spotLights), spotLightCount(spotLightCount),
      projection(projection),
      uniformModel(0), uniformProjection(0),
      uniformView(0), uniformEyePos(0),
      uniformSpecularIntensity(0), uniformShininess(0)
{
}

void Renderer::Initialize(){
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glFrontFace(GL_CCW);
    glViewport(0, 0, window->getBufferWidth(), window->getBufferHeight());
}

void Renderer::Update(float deltaTime){

}

void Renderer::Render() {
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    (*shaders)[0].UseShader();
    uniformModel = (*shaders)[0].GetModelLocation();
    uniformProjection = (*shaders)[0].GetProjectionLocation();
    uniformView = (*shaders)[0].GetViewLocation();
    uniformEyePos = (*shaders)[0].GetEyePosLocation();
    uniformSpecularIntensity = (*shaders)[0].GetSpecularIntensityLocation();
    uniformShininess = (*shaders)[0].GetShininessLocation();

    (*shaders)[0].SetDirectionalLight(mainLight);
    (*shaders)[0].SetPointLights(pointLights, *pointLightCount);
    (*shaders)[0].SetSpotLights(spotLights, *spotLightCount);

    glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(*projection));
    glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera->calculateViewMatrix()));
    glUniform3f(uniformEyePos,
        camera->getCameraPosition().x,
        camera->getCameraPosition().y,
        camera->getCameraPosition().z);

    DrawEntities();
}

void Renderer::DrawEntities(){
    std::vector<int> allEntities = (*ecsManager).GetEntities();

    for (int entityID : allEntities) {
        if ((*ecsManager).HasComponent<TransformComponent>(entityID)) {
            auto& transform = (*ecsManager).GetTransformComponent(entityID);
            glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(transform.GetModelMatrix()));
        }

        if ((*ecsManager).HasComponent<MaterialComponent>(entityID)) {
            (*ecsManager).GetMaterialComponent(entityID).material->UseMaterial(uniformSpecularIntensity, uniformShininess);
        }
        if ((*ecsManager).HasComponent<TextureComponent>(entityID)) {
            (*ecsManager).GetTextureComponent(entityID).texture->UseTexture();
        }

        if ((*ecsManager).HasComponent<MeshComponent>(entityID)) {
            (*ecsManager).GetMeshComponent(entityID).mesh->RenderMesh();
        }
        else if ((*ecsManager).HasComponent<ModelComponent>(entityID)) {
            (*ecsManager).GetModelComponent(entityID).model->RenderModel();
        }
    }
}

void Renderer::Shutdown(){

}
