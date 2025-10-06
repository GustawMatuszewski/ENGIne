#ifndef DIRECTIONALLIGHT_HPP
#define DIRECTIONALLIGHT_HPP

#include "Light.hpp"

class DirectionalLight:
    public Light{
        public:
            DirectionalLight();
            DirectionalLight(GLfloat red, GLfloat green, GLfloat blue, GLfloat aIntensity, GLfloat dIntensity, GLfloat xDir, GLfloat yDir, GLfloat zDir);
            ~DirectionalLight();

            void UseLight(GLfloat ambientIntensityLocation, GLfloat ambientColorLocation, GLfloat diffuseIntensityLocation, GLfloat directionLocation);
            
            glm::vec3 GetDirection() const { return direction; }
            void SetDirection(const glm::vec3& dir) { direction = glm::normalize(dir); }

        private:
            glm::vec3 direction;
    };
#endif