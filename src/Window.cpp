#include "Window.hpp"
#include <string.h>
Window::Window(){
    width = 800;
    height = 800;
    title = "unnamed-window";

    for(size_t i =0; i<1024; i++){
        keys[i] = 0;
    }
}

Window::Window(GLint windowWidth, GLint windowHeight, const char* windowTitle){
    width = windowWidth;
    height = windowHeight;
    title = windowTitle;

    xChange = .0f;
    yChange = .0f;

    for(size_t i =0; i<1024; i++){
        keys[i] = 0;
    }
}

int Window::Initialize(){
    if(!glfwInit()){
        printf("ERROR: GLFW FAILED TO INITIALIZE !!! \n");
        glfwTerminate();
        return 1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    window = glfwCreateWindow(width, height, title, NULL, NULL);
    if (!window) {
        printf("ERROR: COULDN'T CREATE WINDOW !!! \n");
        glfwTerminate();
        return 1;
    }

    glfwGetFramebufferSize(window, &bufferWidth, &bufferHeight);
    glfwMakeContextCurrent(window);

    glfwSwapBuffers(window);

    createCallbacks();

    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK) {
        printf("ERROR: GLEW FAILED TO INITIALIZE !!! \n");
        glfwDestroyWindow(window);
        glfwTerminate();
        return 1;
    }


        glEnable(GL_DEPTH_TEST);
        glViewport(0, 0, bufferWidth, bufferHeight);

        glfwSetWindowUserPointer(window, this);

        return 0;
    }

void Window::createCallbacks(){
    glfwSetKeyCallback(window, handleKeys);
    glfwSetCursorPosCallback(window, handleMouse);
}

GLfloat Window::getXChange(){
    GLfloat theChange = xChange;
    xChange = .0f;
    return theChange;
}

GLfloat Window::getYChange(){
    GLfloat theChange = yChange;
    yChange = .0f;
    return theChange;
}

void Window::handleKeys(GLFWwindow* window, int key, int code, int action, int mode){
    Window* theWindow = static_cast<Window*>(glfwGetWindowUserPointer(window));

    if(key >= 0 && key < 1024){
        if(action == GLFW_PRESS){
            theWindow->keys[key] = true;
        }
        else if (action == GLFW_RELEASE){
            theWindow->keys[key] = false;
        }
    }
}

void Window::handleMouse(GLFWwindow* window, double xPos, double yPos){
    Window* theWindow = static_cast<Window*>(glfwGetWindowUserPointer(window));

    if(theWindow->mouseFirstMoved){
        theWindow->lastX = xPos;
        theWindow->lastY = yPos;
        theWindow->mouseFirstMoved = false;
    }

    theWindow->xChange += xPos - theWindow->lastX;
    theWindow->yChange += theWindow->lastY - yPos;

    theWindow->lastX = xPos;
    theWindow->lastY = yPos;
}

void Window::closeWindow(int key){
    if(getsKeys()[key]){
        glfwSetWindowShouldClose(window, GL_TRUE);
    }
}

bool Window::mouseLockOnWindow(int key){
    static bool mode = false;
    if(getsKeys()[key]){
        mode = !mode;
        if(mode)
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        else
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

        getsKeys()[key] = false;
    }
    return mode;
}

Window::~Window(){
    glfwDestroyWindow(window);
    glfwTerminate();
}