#define STB_IMAGE_IMPLEMENTATION

#include <iostream>
#include <string.h>
#include <vector>
#include <chrono>
#include <thread>

#include "imgui.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>

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
#include "PointLight.hpp"
#include "SpotLight.hpp"
#include "Material.hpp"
#include "Model.hpp"
#include "UI.hpp"

#include "Entity.hpp"
#include "EcsManager.hpp"

ECSManager ecsManager;

Window mainWindow;
Camera mainCamera;
std::vector<Mesh*> meshList;
std::vector<Shader> shaderList;

Texture noTexture;
Texture plainTexture;

Material shinyMaterial;
Material roughMaterial;

Model church;
Entity testEntity;

DirectionalLight mainLight;
PointLight pointLights[MAX_POINT_LIGHTS];
SpotLight spotLights[MAX_SPOT_LIGHTS];

GLfloat deltaTime = .0f;
GLfloat lastTime = .0f;
GLfloat fpsTime = .0f;
GLint frameCount = 0;
int fps = 0;

std::string homePath = "../Home";

static const char* vShader = "../Shaders/default.vert";
static const char* fShader = "../Shaders/default.frag";

float mouseSens = .03f;

void CreateObject(){
    unsigned int indices[]={
        0, 3, 1,
        1, 3, 2,
        2, 3, 0,
        0, 1, 2
    };

    GLfloat vertices[] = {
	//	x      y      z			u	  v         nx    ny   nz
		-1.0f, -1.0f, -0.6f,	0.0f, 0.0f,     .0f, .0f, .0f,
		0.0f, -1.0f, 1.0f,		0.5f, 0.0f,     .0f, .0f, .0f,
		1.0f, -1.0f, -0.6f,		1.0f, 0.0f,     .0f, .0f, .0f,
		0.0f, 1.0f, 0.0f,		0.5f, 1.0f,     .0f, .0f, .0f
	};

    unsigned int floorIndices[] = {
        0, 2, 1,
        1, 2, 3
    };

    GLfloat floorVertices[] = {
        -10.0f, .0f, -10.0f,    .0f, .0f,       .0f, -1.0f, .0f,
        10.0f, .0f, -10.0f,     10.0f, .0f,     .0f, -1.0f, .0f,
        -10.0f, .0f, 10.0f,     .0f, 10.0f,     .0f, -1.0f, .0f,
        10.0f, .0f, 10.0f,      10.0f, 10.0f,   .0f, -1.0f, .0f
    };

    Utils().calcAverageNormals(indices, 12, vertices, 32, 8, 5);

    Mesh *TestObj = new Mesh();
    TestObj->CreateMesh(vertices, indices, 32, 12);
    meshList.push_back(TestObj);

    Mesh *TestFloorObj = new Mesh();
    TestFloorObj->CreateMesh(floorVertices, floorIndices, 32, 6);
    meshList.push_back(TestFloorObj);
}

void CreateShaders(){
    Shader *shader1 = new Shader();
    shader1->CreateFromFiles(vShader, fShader);
    shaderList.push_back(*shader1);
}

int main(){

    mainWindow = Window(1920, 1080, "ENGIne 0.1.1");
    mainWindow.Initialize();

    glfwSwapInterval(0); // Disable VSync set 1 to enable

    const double TARGET_FPS = 144.0; //FPS LIMITER !!!!!!!!!!!!!!
    const double TARGET_FRAME_TIME = 1.0 / TARGET_FPS;

    CreateObject();
    CreateShaders();



    int floorEntity = ecsManager.CreateEntity();

    TransformComponent floorTransform;
    floorTransform.position = glm::vec3(0.0f, -2.0f, 0.0f);
    ecsManager.AddTransformComponent(floorEntity, floorTransform);

    MeshComponent floorMesh;
    floorMesh.mesh = meshList[1]; // Get the floor mesh
    ecsManager.AddMeshComponent(floorEntity, floorMesh);

    TextureComponent floorTexture;
    floorTexture.texture = &plainTexture;
    ecsManager.AddTextureComponent(floorEntity, floorTexture);

    MaterialComponent floorMaterial;
    floorMaterial.material = &shinyMaterial;
    ecsManager.AddMaterialComponent(floorEntity, floorMaterial);
    


    int pyramidEntity = ecsManager.CreateEntity();

    TransformComponent pyramidTransform;
    pyramidTransform.position = glm::vec3(0.0f, 2.0f, 0.0f);
    ecsManager.AddTransformComponent(pyramidEntity, pyramidTransform);

    MeshComponent pyramidMesh;
    pyramidMesh.mesh = meshList[0]; // Get the pyramid mesh
    ecsManager.AddMeshComponent(pyramidEntity, pyramidMesh);

    TextureComponent pyramidTexture;
    pyramidTexture.texture = &noTexture;
    ecsManager.AddTextureComponent(pyramidEntity, pyramidTexture);

    MaterialComponent pyramidMaterial;
    pyramidMaterial.material = &shinyMaterial;
    ecsManager.AddMaterialComponent(pyramidEntity, pyramidMaterial);

    // Create an entity for the church
    
    int churchEntity = ecsManager.CreateEntity();
    TransformComponent churchTransform;
    churchTransform.position = glm::vec3(2.0f, 0.0f, 0.0f);
    churchTransform.rotation.x = -90.0f;
    ecsManager.AddTransformComponent(churchEntity, churchTransform);
  
    ModelComponent churchModel;
    church.LoadModel("../Models/scene.gltf");
    churchModel.model = &church;
    ecsManager.AddModelComponent(churchEntity, churchModel);

    

    //IM GUI INIT

    mainCamera = Camera(glm::vec3(.0f,10.0f,10.0f), glm::vec3(.0f,1.0f,.0f), -90.0f, .0f, 3.0f,5.0f);

    noTexture.LoadTexture2D();

    plainTexture = Texture("../Textures/plain.png");
    plainTexture.LoadTexture2D();

    shinyMaterial = Material(1.0f, 1080);
    roughMaterial = Material(.3f, 4);

    mainLight = DirectionalLight(1.0f,1.0f,1.0f, .1f, .3f, 
                                .0f, .0f, -1.0f);

    unsigned int pointLightCount = 0;
    pointLights[0] = PointLight(1.0f, .0f, .0f, .1f, 1.0f,
                                -4.0f, 2.0f, 2.0f,
                                .3f, .1f, .1f);
    pointLightCount++;

    pointLights[1] = PointLight(.0f, 1.0f, .0f, .1f, 1.0f,
                                .0f, 2.0f, -2.0f,
                                .3f, .1f, .1f);
    pointLightCount++;

    pointLights[2] = PointLight(.0f, .0f, 1.0f, .1f, 1.0f,
                                4.0f, 2.0f, 2.0f,
                                .3f, .1f, .1f);
    pointLightCount++;


    unsigned int spotLightCount = 0;
    spotLights[0] = SpotLight(.0f, 1.0f, .0f, .1f, 1.0f,
                                .0f, 2.0f, .0f,
                                .0f, -1.0f, .0f,
                                1.0f, .0f, .0f,
                                20.0f);
    spotLightCount++;


    GLuint uniformProjection = 0, uniformModel = 0, uniformView = 0, uniformEyePos = 0, uniformSpecularIntensity = 0, uniformShininess = 0;

    glm::mat4 projection = glm::perspective(45.0f, mainWindow.getBufferWidth()/mainWindow.getBufferHeight(), .1f, 100.0f);

    glEnable(GL_DEPTH_TEST);

    //IM GUI INIT    
    UI::Init(mainWindow.getGLFWwindow());
    
    while(!mainWindow.getShouldClose()){

        double frameStartTime = glfwGetTime();

        GLfloat now = glfwGetTime();
        deltaTime = now - lastTime;
        lastTime = now;

        frameCount++;
        if (now - fpsTime >= 1.0) {
            fps = frameCount;
            frameCount = 0;
            fpsTime = now;
        }

        //Window Control

        glfwPollEvents();

        ImGuiIO& io = ImGui::GetIO();

        if (!io.WantCaptureKeyboard) {
            mainCamera.keyControl(mainWindow.getsKeys(), deltaTime);
        }

        bool isMouseLocked = mainWindow.mouseLockOnWindow(GLFW_KEY_ESCAPE);

        if (isMouseLocked && !io.WantCaptureMouse) {
            mainCamera.mouseControl(mainWindow.getXChange() * mouseSens, mainWindow.getYChange() * mouseSens);
        } else {
            mainCamera.mouseControl(0, 0);
        }

        mainWindow.closeWindow(GLFW_KEY_X);
        //-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-==-=


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

        //Draw Entities
        std::vector<int> allEntities = ecsManager.GetEntities();

        for (int entityID : allEntities) {
            // Transform
            if (ecsManager.HasComponent<TransformComponent>(entityID)) {
                auto& transform = ecsManager.GetTransformComponent(entityID);
                glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(transform.GetModelMatrix()));
            }

            // Material + Texture
            if (ecsManager.HasComponent<MaterialComponent>(entityID)) {
                ecsManager.GetMaterialComponent(entityID).material->UseMaterial(uniformSpecularIntensity, uniformShininess);
            }
            if (ecsManager.HasComponent<TextureComponent>(entityID)) {
                ecsManager.GetTextureComponent(entityID).texture->UseTexture();
            }

            // Mesh OR Model
            if (ecsManager.HasComponent<MeshComponent>(entityID)) {
                ecsManager.GetMeshComponent(entityID).mesh->RenderMesh();
            }
            else if (ecsManager.HasComponent<ModelComponent>(entityID)) {
                ecsManager.GetModelComponent(entityID).model->RenderModel();
            }
        }




        glDisable(GL_DEPTH_TEST);

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        //Place all the elements  here
        UI::FpsCounter(fps, 10, 10, 100, 50);

        UI::InspectorPanel(ecsManager, floorEntity, 10, 70, 300, 300);
        UI::InspectorPanel(ecsManager, pyramidEntity, 10, 390, 300, 300);
        UI::InspectorPanel(ecsManager, churchEntity, 10, 710, 300, 300);

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glEnable(GL_DEPTH_TEST);

        glUseProgram(0);

        mainWindow.SwapBuffers();

        double frameEndTime = glfwGetTime();
        double elapsedTime = frameEndTime - frameStartTime;

        if (elapsedTime < TARGET_FRAME_TIME) {
            double sleepTime = TARGET_FRAME_TIME - elapsedTime;
            std::this_thread::sleep_for(std::chrono::microseconds(static_cast<int>(sleepTime * 1000000)));
        }
    }

    
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    return 0;
}