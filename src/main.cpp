
#include <iostream>
#include <string.h>
#include <vector>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <assimp/Importer.hpp>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "CommonValues.hpp"
#include "Utils.hpp"

#include "Mesh.hpp"
#include "Shader.hpp"
#include "Window.hpp"
#include "Camera.hpp"
#include "DefaultScene.h" //<-------
#include "Texture.hpp"
#include "DirectionalLight.hpp"
#include "EntityManager.h"
#include "PointLight.hpp"
#include "SpotLight.hpp"
#include "Material.hpp"
#include "Model.hpp"
#include "EntityManager.h"
#include "Entity.h"

const float toRadians = 3.141592265f/180.0f;

DefaultScene scene;
Window *mainWindow;
Camera *mainCamera ;
std::vector<Shader> shaderList;

Texture noTexture;
Texture plainTexture;

//Material shinyMaterial;
Material roughMaterial;

Model test;

//DirectionalLight mainLight;
PointLight pointLights[MAX_POINT_LIGHTS];
SpotLight spotLights[MAX_SPOT_LIGHTS];

GLfloat deltaTime = .0f;
GLfloat lastTime = .0f;

static const char* vShader = "../Shaders/default.vert";
static const char* fShader = "../Shaders/default.frag";

//TEST
EntityManager entityManager;


// void CreateShaders(){
//     auto shader1 = new Shader();
//     shader1->CreateFromFiles(vShader, fShader);
//     shaderList.push_back(*shader1);
// }

int main(){
    mainWindow = &scene.mainWindow;
    mainCamera = &scene.mainCamera;
    // CreateShaders();
    shaderList.push_back(scene.assetManager.shaders["nyger"]);
    noTexture.LoadTexture2D();

    plainTexture = Texture("../Textures/KJ.png");
    plainTexture.LoadTexture2D_A();

    auto shinyMaterial = scene.assetManager.CreateMaterial("shinyMaterial",1.0f, 1080);
    roughMaterial = Material(.3f, 4);

    //mainLight = DirectionalLight(1.0f,1.0f,1.0f, .1f, .3f,
     //                           .0f, -1.0f, -.4f);

    unsigned int pointLightCount = 0;
    unsigned int spotLightCount = 0;
    /*
    pointLights[0] = PointLight(1.0f, 1.0f, 1.0f, .1f, 1.0f,
                                -4.0f, 2.0f, 2.0f,
                                .3f, .1f, .1f);
    pointLightCount++;

    pointLights[1] = PointLight(1.0f, 1.0f, 1.0f, .1f, 1.0f,
                                .0f, 2.0f, -2.0f,
                                .3f, .1f, .1f);
    pointLightCount++;

    pointLights[2] = PointLight(1.0f, 1.0f, 1.0f, .1f, 1.0f,
                                4.0f, 2.0f, 2.0f,
                                .3f, .1f, .1f);
    pointLightCount++;


    spotLights[0] = SpotLight(1.0f, 1.0f, 1.0f, .1f, 1.0f,
                                .0f, 2.0f, .0f,
                                .0f, -1.0f, .0f,
                                1.0f, .0f, .0f,
                                20.0f);
    spotLightCount++;


    */
    GLuint uniformProjection = 0, uniformModel = 0, uniformView = 0, uniformEyePos = 0, uniformSpecularIntensity = 0, uniformShininess = 0;

    glm::mat4 projection = glm::perspective(45.0f, mainWindow->getBufferWidth()/mainWindow->getBufferHeight(), .1f, 100.0f);

    glEnable(GL_DEPTH_TEST);

    //TEST ENTITY
    auto entity = new Entity();
    entityManager.AddComponent(entity, new TransformComponent());
    entityManager.AddComponent(entity, new ModelComponent());
    entityManager.AddComponent(entity, new TextureComponent());
    entityManager.AddComponent(entity, new DirectionalLightComponent());

    auto transform = entityManager.GetComponent<TransformComponent>(entity);
    auto modelE = entityManager.GetComponent<ModelComponent>(entity);
    auto textureE = entityManager.GetComponent<TextureComponent>(entity);
    auto dLight = entityManager.GetComponent<DirectionalLightComponent>(entity);

    transform->position = glm::vec3(0.0f);
    transform->rotation.y = 90.0f;
    transform->scale = glm::vec3(1.0f);

    textureE->texture = &plainTexture;

    modelE->modelPath = "../Models/Primitives/Sphere.glb";
    modelE->model->LoadModel(modelE->modelPath);

    dLight->color=glm::vec3(1.0f, 1.0f, 1.0f);
    dLight->ambientIntensity = .1f;
    dLight->diffuseIntensity = .3f;
    dLight->direction=glm::vec3(.0f,.0f,1.0f);
    dLight->UpdateDirectionalLight();

    auto planeE = new Entity();
    entityManager.AddComponent(planeE, new ModelComponent());

    auto plane = entityManager.GetComponent<ModelComponent>(planeE);
    auto planeTransform = entityManager.GetComponent<TransformComponent>(planeE);
    auto planeTexture = entityManager.GetComponent<TextureComponent>(planeE);

    plane->modelPath = "../Models/Primitives/Plane.glb";
    plane->model->LoadModel(plane->modelPath);

    planeTexture->texture = &plainTexture;

    planeTransform->position = glm::vec3(0.0f);
    planeTransform->rotation.y = -90.0f;
    planeTransform->scale = glm::vec3(10.0f);

    auto pointLightEntity = new Entity();
    entityManager.AddComponent(pointLightEntity,new PointLightComponent());

    auto pLight = entityManager.GetComponent<PointLightComponent>(pointLightEntity);

    pLight->position =  glm::vec3(.0f, 2.0f, .0f);
    pLight->color =  glm::vec3(1.0f, 1.0f, 1.0f);
    pLight->ambientIntensity = .1f;
    pLight->diffuseIntensity = .1f;


    pLight->Update();
    pointLights[pointLightCount] = *pLight->GetPointLight();
    pointLightCount++;

#include <typeinfo>

    for (auto* c : pointLightEntity->components) {
        printf("Component type: %s\n", typeid(*c).name());
    }


    while(!mainWindow->getShouldClose()){
        GLfloat now = glfwGetTime();
        deltaTime = now - lastTime;
        lastTime = now;

        glfwPollEvents();

        mainCamera->keyControl(mainWindow->getsKeys(), deltaTime);
        mainCamera->mouseControl(mainWindow->getXChange(), mainWindow->getYChange(), deltaTime);

        glClearColor(.1f, .1f ,.1f ,.7f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        shaderList[0].UseShader();
        uniformModel = shaderList[0].GetModelLocation();
        uniformProjection = shaderList[0].GetProjectionLocation();
        uniformView = shaderList[0].GetViewLocation();
        uniformEyePos = shaderList[0].GetEyePosLocation();
        uniformSpecularIntensity = shaderList[0].GetSpecularIntensityLocation();
        uniformShininess = shaderList[0].GetShininessLocation();

        shaderList[0].SetDirectionalLight(dLight->GetDirectionalLight());
        shaderList[0].SetPointLights(pointLights, pointLightCount);
        shaderList[0].SetSpotLights(spotLights, spotLightCount);

        glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
        glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(mainCamera->calculateViewMatrix()));
        glUniform3f(uniformEyePos, mainCamera->getCameraPosition().x, mainCamera->getCameraPosition().y, mainCamera->getCameraPosition().z);

        //transform->position.x += .001;
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        // Update rotation
        transform->rotation.x += 20.0f * deltaTime; // rotate 20 degrees per second on X
        transform->rotation.y += 30.0f * deltaTime; // rotate 30 degrees per second on Y
        transform->rotation.z += 15.0f * deltaTime; // rotate 15 degrees per second on Z

        // Keep angles between 0-360 for cleanliness
        if(transform->rotation.x > 360.0f) transform->rotation.x -= 360.0f;
        if(transform->rotation.y > 360.0f) transform->rotation.y -= 360.0f;
        if(transform->rotation.z > 360.0f) transform->rotation.z -= 360.0f;

        // === Animate Point Light ===

        // Move the light over time
        pLight->position.x += 1.2f * deltaTime;
        pLight->position.y += 0.8f * deltaTime;
        pLight->position.z += 1.5f * deltaTime;

        // Wrap the movement inside [-3, 3]
        if (pLight->position.x > 3.0f) pLight->position.x = -3.0f;
        if (pLight->position.x < -3.0f) pLight->position.x = 3.0f;

        if (pLight->position.y > 3.0f) pLight->position.y = -3.0f;
        if (pLight->position.y < -3.0f) pLight->position.y = 3.0f;

        if (pLight->position.z > 3.0f) pLight->position.z = -3.0f;
        if (pLight->position.z < -3.0f) pLight->position.z = 3.0f;

        // Update ECS component and sync to renderer array
        pLight->Update();
        pointLights[0] = *pLight->GetPointLight();

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

        glm::mat4 model(1.0f);
        model = glm::translate(model, transform->position);

        model = glm::rotate(model, glm::radians(transform->rotation.x), glm::vec3(1,0,0));
        model = glm::rotate(model, glm::radians(transform->rotation.y), glm::vec3(0,1,0));
        model = glm::rotate(model, glm::radians(transform->rotation.z), glm::vec3(0,0,1));

        model = glm::scale(model, transform->scale);

        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));

        shinyMaterial.UseMaterial(uniformSpecularIntensity, uniformShininess);

        textureE->textures=std::vector<Texture*>(modelE->model->GetMeshCount(), nullptr);
        textureE->textures[0] = &plainTexture;
        modelE->model->RenderModel(textureE->textures);

        //plane
        model = glm::mat4(1.0f);
        model = glm::translate(model, planeTransform->position);

        model = glm::rotate(model, glm::radians(planeTransform->rotation.x), glm::vec3(1,0,0));
        model = glm::rotate(model, glm::radians(planeTransform->rotation.y), glm::vec3(0,1,0));
        model = glm::rotate(model, glm::radians(planeTransform->rotation.z), glm::vec3(0,0,1));

        model = glm::scale(model, planeTransform->scale);

        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));

        shinyMaterial.UseMaterial(uniformSpecularIntensity, uniformShininess);

        planeTexture->textures=std::vector<Texture*>(modelE->model->GetMeshCount(), nullptr);
        planeTexture->textures[0] = &plainTexture;
        plane->model->RenderModel(planeTexture->textures);



        glUseProgram(0);

        mainWindow->SwapBuffers();
    }
    return 0;
}