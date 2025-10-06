#ifndef SPOTLIGHT_HPP
#define SPOTLIGHT_HPP

#include "PointLight.hpp"

class SpotLight:
    public PointLight{
        public:
            SpotLight(); 

            SpotLight(GLfloat red, GLfloat green, GLfloat blue,
                        GLfloat aIntensity, GLfloat dIntensity,
                        GLfloat xPos, GLfloat yPos, GLfloat zPos,
                        GLfloat xDir, GLfloat yDir, GLfloat zDir,
                        GLfloat con, GLfloat lin, GLfloat exp,
                        GLfloat edge);

            void UseLight(GLuint ambientIntensityLocation, GLuint ambientColorLocation, GLuint diffuseIntensityLocation,
                            GLuint positionLocation,  GLuint directionLocation, GLuint constantLocation,
                            GLuint linearLocation, GLuint exponentLocation, GLuint edgeLocation);

            void SetFlash(glm::vec3 pos, glm::vec3 dir);

            ~SpotLight();

            glm::vec3 GetDirection() const { return direction; }
            GLfloat GetEdge() const { return edge; }
            GLfloat GetProcEdge() const { return procEdge; }

            void SetDirection(glm::vec3 direction) { this->direction = direction; }
            void SetEdge(GLfloat edge) { this->edge = edge; }
            void SetProcEdge(GLfloat procEdge) { this->procEdge = procEdge; }

        private:
            glm::vec3 direction;
            
            GLfloat edge, procEdge;
    };
#endif