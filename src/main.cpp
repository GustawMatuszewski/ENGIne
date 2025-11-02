#define STB_IMAGE_IMPLEMENTATION

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

Window mainWindow;
Camera mainCamera;
std::vector<Mesh*> meshList;
std::vector<Shader> shaderList;

Texture noTexture;
Texture plainTexture;

Material shinyMaterial;
Material roughMaterial;

Model test;

DirectionalLight mainLight;
PointLight pointLights[MAX_POINT_LIGHTS];
SpotLight spotLights[MAX_SPOT_LIGHTS];

GLfloat deltaTime = .0f;
GLfloat lastTime = .0f;

static const char* vShader = "../Shaders/default.vert";
static const char* fShader = "../Shaders/default.frag";

//TEST
EntityManager entityManager;


void CreateShaders(){
    Shader *shader1 = new Shader();
    shader1->CreateFromFiles(vShader, fShader);
    shaderList.push_back(*shader1);
}

int main(){

    mainWindow = Window(1366, 768, "ENGine 0.1");
    mainWindow.Initialize();

    CreateShaders();

    mainCamera = Camera(glm::vec3(.0f,.0f,.0f), glm::vec3(.0f,1.0f,.0f), -90.0f, .0f, 3.0f,5.0f);

    noTexture.LoadTexture2D();

    plainTexture = Texture("../Textures/plain.png");
    plainTexture.LoadTexture2D();

    shinyMaterial = Material(1.0f, 1080);
    roughMaterial = Material(.3f, 4);

    mainLight = DirectionalLight(1.0f,1.0f,1.0f, .1f, .3f,
                                .0f, -1.0f, -.4f);

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

    glm::mat4 projection = glm::perspective(45.0f, mainWindow.getBufferWidth()/mainWindow.getBufferHeight(), .1f, 100.0f);

    glEnable(GL_DEPTH_TEST);

    //TEST ENTITY
    Entity* entity = new Entity();
    entityManager.AddComponent(entity, new TransformComponent());
    entityManager.AddComponent(entity, new ModelComponent());
    entityManager.AddComponent(entity, new TextureComponent());

    TransformComponent* transform = entityManager.GetComponent<TransformComponent>(entity);
    ModelComponent* modelE = entityManager.GetComponent<ModelComponent>(entity);
    TextureComponent* textureE = entityManager.GetComponent<TextureComponent>(entity);

    transform->position = glm::vec3(0.0f);
    textureE->texture = &plainTexture;
    modelE->modelPath = "../Models/Primitives/Sphere.glb";
    modelE->model->LoadModel(modelE->modelPath);

#include <typeinfo>

    for (auto* c : entity->components) {
        printf("Component type: %s\n", typeid(*c).name());
    }


    while(!mainWindow.getShouldClose()){
        GLfloat now = glfwGetTime();
        deltaTime = now - lastTime;
        lastTime = now;

        glfwPollEvents();

        mainCamera.keyControl(mainWindow.getsKeys(), deltaTime);
        mainCamera.mouseControl(mainWindow.getXChange(), mainWindow.getYChange(), deltaTime);

        glClearColor(.0f, .0f ,.0f ,1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        shaderList[0].UseShader();
        uniformModel = shaderList[0].GetModelLocation();
        uniformProjection = shaderList[0].GetProjectionLocation();
        uniformView = shaderList[0].GetViewLocation();
        uniformEyePos = shaderList[0].GetEyePosLocation();
        uniformSpecularIntensity = shaderList[0].GetSpecularIntensityLocation();
        uniformShininess = shaderList[0].GetShininessLocation();

        shaderList[0].SetDirectionalLight(&mainLight);
        shaderList[0].SetPointLights(pointLights, pointLightCount);
        shaderList[0].SetSpotLights(spotLights, spotLightCount);

        glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
        glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(mainCamera.calculateViewMatrix()));
        glUniform3f(uniformEyePos, mainCamera.getCameraPosition().x, mainCamera.getCameraPosition().y, mainCamera.getCameraPosition().z);

        transform->position.x += .001;

        glm::mat4 model(1.0f);
        model = glm::translate(model, transform->position);
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));

        shinyMaterial.UseMaterial(uniformSpecularIntensity, uniformShininess);

        textureE->textures=std::vector<Texture*>(modelE->model->GetMeshCount(), nullptr);
        textureE->textures[0] = &plainTexture;
        modelE->model->RenderModel(textureE->textures);



        glUseProgram(0);

        mainWindow.SwapBuffers();
    }
    return 0;
}