#ifndef ENTITY_HPP
#define ENTITY_HPP

#include <glm/glm.hpp>

#include "Camera.hpp"
#include "Texture.hpp"
#include "DirectionalLight.hpp"
#include "PointLight.hpp"
#include "SpotLight.hpp"
#include "Material.hpp"
#include "Model.hpp"
#include "Utils.hpp"

#include "EcsManager.hpp"

#include <glm/glm.hpp>


const int toRadians = Utils().toRadians;

class Entity{
    public:


};

class TransformComponent {
    public:
        TransformComponent()
            : position(0.0f), rotation(0.0f), scale(1.0f), modelMatrix(1.0f), dirty(true) {}

        void SetPosition(const glm::vec3& pos) { position = pos; dirty = true; }
        void SetRotation(const glm::vec3& rot) { rotation = rot; dirty = true; }
        void SetScale(const glm::vec3& scl)    { scale = scl; dirty = true; }
        void SetDirty() { dirty = true; }

        const glm::mat4& GetModelMatrix() {
            if (dirty) {
                modelMatrix = glm::mat4(1.0f);
                modelMatrix = glm::translate(modelMatrix, position);
                modelMatrix = glm::rotate(modelMatrix, glm::radians(rotation.x), {1,0,0});
                modelMatrix = glm::rotate(modelMatrix, glm::radians(rotation.y), {0,1,0});
                modelMatrix = glm::rotate(modelMatrix, glm::radians(rotation.z), {0,0,1});
                modelMatrix = glm::scale(modelMatrix, scale);
                dirty = false;
            }
            return modelMatrix;
        }

        glm::vec3 position;
        glm::vec3 rotation;
        glm::vec3 scale;

    private:
        glm::mat4 modelMatrix;
        bool dirty;
};


class MeshComponent {
    public:
        Mesh* mesh;
};

class ModelComponent {
    public:
        Model* model;
};

class TextureComponent {
    public:
        Texture* texture;

        GLuint getTextureID() const { 
            if (texture) {
                return texture->GetTextureID(); 
            }
            return 0;
        }
};

class MaterialComponent {
    public:
        Material* material;
};

class LightComponent {
    public:
        DirectionalLight* dirLight;
        PointLight* pointLight;
        SpotLight* spotLight;
};

class CameraComponent {
    public:
        Camera* camera;
};

class DirectionalLightComponent{
    public:
        DirectionalLight* directionalLight;
};

class PointLightComponent{
    public:
        PointLight* pointLight;
};

class SpotLightComponent{
    public:
        SpotLight* spotLight;
};

class ScriptComponent {
    public:
        

};

#endif