#ifndef ENGINE_ENTITYMANAGER_H
#define ENGINE_ENTITYMANAGER_H

#include <glm/glm.hpp>

#include "DirectionalLight.hpp"
#include "PointLight.hpp"
#include "Entity.h"
#include "Model.hpp"

class EntityManager {
    public:
        void AddComponent(Entity* entity, Component* component);
        void RemoveComponent(Entity* entity, Component* component);

        template<typename T>
        T* GetComponent(Entity* entity) {
            for (Component* comp : entity->components) {
                if (T* match = dynamic_cast<T*>(comp)) {
                    return match;
                }
            }
            return nullptr;
        }
};

class Component {
    public:
        virtual ~Component() = default;
};

class TransformComponent : public Component {
    public:
        glm::vec3 position;
        glm::vec3 rotation;
        glm::vec3 scale;
};

class TextureComponent : public Component {
    public:
        Texture* texture;
        std::vector<Texture*> textures;//So it works with children
        std::string filename;

};

class ModelComponent : public Component {
    public:
        Model* model = new Model();
        std::string modelPath;
};

class DirectionalLightComponent : public Component {
    public:

        //Default light Values
        glm::vec3 color = glm::vec3(1.0f, 1.0f, 1.0f);
        float ambientIntensity, diffuseIntensity = .1f;
        glm::vec3 direction = glm::vec3(0.0f, -1.0f, 0.0f);

        void UpdateDirectionalLight() {
            directionalLight = new DirectionalLight(color.x, color.y, color.z,
            ambientIntensity, diffuseIntensity, direction.x, direction.y, direction.z);
        }

        DirectionalLight* GetDirectionalLight(){return directionalLight;}

    private:
        DirectionalLight* directionalLight = new DirectionalLight(color.x, color.y, color.z,
            ambientIntensity, diffuseIntensity, direction.x, direction.y, direction.z);
};

class PointLightComponent : public Component {
    public:
        glm::vec3 position = glm::vec3(.0f, 3.0f, .0f);
        glm::vec3 color = glm::vec3(1.0f, .0f, .0f);
        float ambientIntensity = 0.1f;
        float diffuseIntensity = 0.1f;
        float con, lin, exp = .1f;

        ~PointLightComponent() {
            delete pointLight;
        }

        void Update() {
            pointLight = new PointLight(color.x, color.y, color.z,
                                    ambientIntensity, diffuseIntensity,
                                            position.x,
                                            position.y,
                                            position.z,
                                            con,lin,exp);
        }

        PointLight* GetPointLight() { return pointLight; }

    private:
        PointLight* pointLight = new PointLight(color.x, color.y, color.z,
                                            ambientIntensity, diffuseIntensity,
                                                    position.x,
                                                    position.y,
                                                    position.z,
                                                    con,lin,exp);
};

class SpotLightComponent : public Component {
    public:

};

#endif //ENGINE_ENTITYMANAGER_H