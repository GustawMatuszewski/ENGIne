#ifndef POINTLIGHT_HPP
#define POINTLIGHT_HPP

#include "Light.hpp"

class PointLight:
    public Light{
        public:
            PointLight(); 
            PointLight(GLfloat red, GLfloat green, GLfloat blue,
                        GLfloat aIntensity, GLfloat dIntensity,
                        GLfloat xPos, GLfloat yPos, GLfloat zPos,
                        GLfloat con, GLfloat lin, GLfloat exp);

            void UseLight(GLuint ambientIntensityLocation, GLuint ambientColorLocation, GLuint diffuseIntensityLocation,
                            GLuint positionLocation, GLuint constantLocation,
                            GLuint linearLocation, GLuint exponentLocation);

            ~PointLight();

            glm::vec3 GetPosition() const { return position; }
            GLfloat GetConstant() const { return constant; }
            GLfloat GetLinear() const { return linear; }
            GLfloat GetExponent() const { return exponent; }

            void SetPosition(glm::vec3 pos) { position = pos; }
            void SetConstant(GLfloat con) { constant = con; }
            void SetLinear(GLfloat lin) { linear = lin; }
            void SetExponent(GLfloat exp) { exponent = exp; }

        protected:
            glm::vec3 position;

            GLfloat constant, linear, exponent;            
    };
#endif