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
#include "PointLight.hpp"
#include "SpotLight.hpp"
#include "Material.hpp"
#include "Model.hpp"


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

    mainWindow = Window(1366, 768, "ENGine 0.1");
    mainWindow.Initialize();

    CreateObject();
    CreateShaders();

    mainCamera = Camera(glm::vec3(.0f,.0f,.0f), glm::vec3(.0f,1.0f,.0f), -90.0f, .0f, 3.0f,5.0f);

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

        glm::mat4 model(1.0f);
        model = glm::translate(model, glm::vec3(.0f, .0f, .0f));
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
        plainTexture.UseTexture();
        shinyMaterial.UseMaterial(uniformSpecularIntensity, uniformShininess);
        meshList[0]->RenderMesh();

        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(.0f, -4.0f, .0f));
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
        plainTexture.UseTexture();
        roughMaterial.UseMaterial(uniformSpecularIntensity, uniformShininess);
        meshList[1]->RenderMesh();

        glUseProgram(0);

        mainWindow.SwapBuffers();
    }

    return 0;
}