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
#include "Renderer.hpp"

#include "Entity.hpp"
#include "EcsManager.hpp"

Window mainWindow;
Camera mainCamera;
std::vector<Mesh*> meshList;
std::vector<Shader> shaderList;

DirectionalLight mainLight;
PointLight pointLights[MAX_POINT_LIGHTS];
SpotLight spotLights[MAX_SPOT_LIGHTS];

ECSManager ecsManager;

Renderer renderer;



Texture noTexture;
Texture plainTexture("../Textures/white.jpg");
Texture testTexture("../Textures/1x1GridOrange.png");

Material shinyMaterial;
Material roughMaterial;

Model plane; //Deafult Primitive Shapes
Model cube;
Model sphere;

Model church;


GLfloat deltaTime = .0f;
GLfloat lastTime = .0f;
GLfloat fpsTime = .0f;
GLint frameCount = 0;
int fps = 0;

std::string homePath = "../Home";

static const char* vShader = "../Shaders/default.vert";
static const char* fShader = "../Shaders/default.frag";

float mouseSens = .03f;


void CreateShaders(){
    Shader *shader1 = new Shader();
    shader1->CreateFromFiles(vShader, fShader);
    shaderList.push_back(*shader1);
}

void effect(TransformComponent& transform) {
    static float rotationAngle = 0.0f;

    rotationAngle += 0.1f;

    transform.rotation = glm::vec3(0.0f, rotationAngle, 0.0f);

    float orbitRadius = 2.0f;
    float orbitSpeed = 0.05f;
    float orbitAngle = rotationAngle * orbitSpeed;

    float x = orbitRadius * cos(orbitAngle);
    float y = .0f;
    float z = orbitRadius * sin(orbitAngle);

    transform.position = glm::vec3(x, y, z);
    transform.SetDirty();
}

int main(){

    mainWindow = Window(1920, 1080, "ENGIne 0.1.1");
    mainWindow.Initialize();

    glfwSwapInterval(1); // Disable VSync set 1 to enable

    const double TARGET_FPS = 144.0; //FPS LIMITER !!!!!!!!!!!!!!
    const double TARGET_FRAME_TIME = 1.0 / TARGET_FPS;

    CreateShaders();

    noTexture.LoadTexture2D();
    plainTexture.LoadTexture2D();
    testTexture.LoadTexture2D_A();

    shinyMaterial = Material(1.0f, 1080);
    roughMaterial = Material(.3f, 4);

    int floorEntity = ecsManager.CreateEntity();

    TransformComponent floorTransform;
    floorTransform.position = glm::vec3(0.0f, -2.0f, 0.0f);
    floorTransform.scale = glm::vec3(1.0f, 1.0f, 1.0f);
    ecsManager.AddTransformComponent(floorEntity, floorTransform);

    ModelComponent floorModel;
    plane.LoadModel("../Models/Primitives/Plane.glb");
    floorModel.model = &plane;
    ecsManager.AddModelComponent(floorEntity, floorModel);

    TextureComponent floorTexture;
    floorTexture.texture = &testTexture;
    ecsManager.AddTextureComponent(floorEntity, floorTexture);

    MaterialComponent floorMaterial;
    floorMaterial.material = &shinyMaterial;
    ecsManager.AddMaterialComponent(floorEntity, floorMaterial);
    

    int sphereEntity = ecsManager.CreateEntity();

    TransformComponent sphereTransform;
    sphereTransform.position = glm::vec3(0.0f, 2.0f, 0.0f);
    ecsManager.AddTransformComponent(sphereEntity, sphereTransform);

    TextureComponent sphereTexture;
    sphereTexture.texture = &plainTexture;
    ecsManager.AddTextureComponent(sphereEntity, sphereTexture);

    ModelComponent sphereModel;
    sphere.LoadModel("../Models/Primitives/Sphere.glb");
    sphereModel.model = &sphere;    
    ecsManager.AddModelComponent(sphereEntity, sphereModel);

    MaterialComponent sphereMaterial;
    sphereMaterial.material = &roughMaterial;
    ecsManager.AddMaterialComponent(sphereEntity, sphereMaterial);
    


    int churchEntity = ecsManager.CreateEntity();

    TransformComponent churchTransform;
    churchTransform.position = glm::vec3(1.0f, -3.0f, -0.20f);
    churchTransform.rotation.x = -90.0f;
    ecsManager.AddTransformComponent(churchEntity, churchTransform);
  
    ModelComponent churchModel;
    church.LoadModel("../Models/scene.gltf");
    churchModel.model = &church;
    ecsManager.AddModelComponent(churchEntity, churchModel);


    int mainLightEntity = ecsManager.CreateEntity();

    DirectionalLightComponent mainLightComp;
    DirectionalLight mainLight(
        1.0f, 1.0f, 1.0f, 0.1f, 0.1f,
        .0f,
        -1.0f,
        .0f
    );
    mainLightComp.directionalLight = &mainLight;
    ecsManager.AddDirectionalLightComponent(mainLightEntity, mainLightComp);


    unsigned int pointLightCount = 0;
    unsigned int spotLightCount = 0;

    int pointLight1Entity = ecsManager.CreateEntity();

    PointLightComponent pointLight1Comp;
    pointLights[0] = PointLight(1.0f, .0f, .0f, .1f, 1.0f,
                                -4.0f, 2.0f, 2.0f,
                                .3f, .1f, .1f);
    pointLight1Comp.pointLight = &pointLights[0];
    ecsManager.AddPointLightComponent(pointLight1Entity, pointLight1Comp);
    pointLightCount++;

    int pointLight2Entity = ecsManager.CreateEntity();

    PointLightComponent pointLight2Comp;
    pointLights[1] = PointLight(.0f, 1.0f, .0f, .1f, 1.0f,
                                .0f, 2.0f, -2.0f,
                                .3f, .1f, .1f);
    pointLight2Comp.pointLight = &pointLights[1];
    ecsManager.AddPointLightComponent(pointLight2Entity, pointLight2Comp);
    pointLightCount++;

    int pointLight3Entity = ecsManager.CreateEntity();

    PointLightComponent pointLight3Comp;
    pointLights[2] = PointLight(.0f, .0f, 1.0f, .1f, 1.0f,
                                4.0f, 2.0f, 2.0f,
                                .3f, .1f, .1f);
    pointLight3Comp.pointLight = &pointLights[2];
    ecsManager.AddPointLightComponent(pointLight3Entity, pointLight3Comp);
    pointLightCount++;

    int spotLight1Entity = ecsManager.CreateEntity();

    SpotLightComponent spotLight1Comp;
    spotLights[0] = SpotLight(.0f, .0f, .0f, .1f, 1.0f,
                                .0f, 2.0f, .0f,
                                .0f, -1.0f, .0f,
                                1.0f, .0f, .0f,
                                20.0f);
    spotLight1Comp.spotLight = &spotLights[0];
    ecsManager.AddSpotLightComponent(spotLight1Entity, spotLight1Comp);
    spotLightCount++;


    mainCamera = Camera(glm::vec3(.0f,10.0f,10.0f), glm::vec3(.0f,1.0f,.0f), -90.0f, .0f, 3.0f,5.0f);

    GLuint uniformProjection = 0, uniformModel = 0, uniformView = 0, uniformEyePos = 0, uniformSpecularIntensity = 0, uniformShininess = 0;

    float aspect = (float)mainWindow.getBufferWidth() / (float)mainWindow.getBufferHeight();
    glm::mat4 projection = glm::perspective(glm::radians(45.0f), aspect, 0.1f, 100.0f);

    renderer = Renderer(&mainWindow, &mainCamera, &ecsManager,
                        &shaderList,
                        &mainLight,
                        pointLights, &pointLightCount,
                        spotLights, &spotLightCount,
                        &projection);

    renderer.Initialize();

    glEnable(GL_DEPTH_TEST);
 
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

        ImGuiIO& io = ImGui::GetIO(); //THIS IS SHIT NEEDS TO BE FIXED!!!!

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


        effect(ecsManager.GetTransformComponent(sphereEntity));
        
        renderer.Render();
        
        glDisable(GL_DEPTH_TEST);

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        //Place all the elements  here
        UI::FpsCounter(fps, 10, 10, 100, 50);

        UI::InspectorPanel(ecsManager, floorEntity, 10, 70, 300, 300);
        UI::InspectorPanel(ecsManager, sphereEntity, 10, 390, 300, 300);
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