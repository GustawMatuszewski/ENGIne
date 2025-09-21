#ifndef WINDOW_HPP
#define WINDOW_HPP

#include <stdio.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

class Window{
    public:
        Window();

        Window(GLint windowWidth, GLint windowHeight, const char* windowTitle);
        GLFWwindow* getGLFWwindow() { return window; }
        
        int Initialize();

        GLfloat getBufferWidth()  { return bufferWidth; }
        GLfloat getBufferHeight() { return bufferHeight; }

        bool getShouldClose()   { return glfwWindowShouldClose(window); }

        bool* getsKeys()    { return keys; }
        
        GLfloat getXChange();
        GLfloat getYChange();

        void closeWindow(int key);
        bool mouseLockOnWindow(int key);

        void SwapBuffers()      { glfwSwapBuffers(window); }

        ~Window();
    private:
        GLFWwindow *window;

        const char* title;
        GLint width, height;
        
        GLint bufferWidth, bufferHeight;

        bool keys[1024];

        GLfloat lastX;
        GLfloat lastY;
        GLfloat xChange;
        GLfloat yChange;
        bool mouseFirstMoved;

        void createCallbacks();
        static void handleKeys(GLFWwindow* window, int key, int code, int action, int mode);
        static void handleMouse(GLFWwindow* window, double xPos, double yPos);
};

#endif