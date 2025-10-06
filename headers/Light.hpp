#ifndef LIGHT_HPP
#define LIGHT_HPP

#include <GL/glew.h>
#include <glm/glm.hpp>

class Light{
    public:
        Light();
        Light(GLfloat red, GLfloat green, GLfloat blue, GLfloat aIntensity, GLfloat dIntensity);
        ~Light();
    
    glm::vec3 GetColor() const { return color; }
    void SetColor(const glm::vec3& newColor) { color = newColor; }

    float GetAmbientIntensity() const { return ambientIntensity; }
    void SetAmbientIntensity(float value) { ambientIntensity = value; }

    float GetDiffuseIntensity() const { return diffuseIntensity; }
    void SetDiffuseIntensity(float value) { diffuseIntensity = value; }


    protected:
        glm::vec3 color;
        GLfloat ambientIntensity;
        GLfloat diffuseIntensity;
};


#endif 