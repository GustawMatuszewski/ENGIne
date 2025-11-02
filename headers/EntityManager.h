#ifndef ENGINE_ENTITYMANAGER_H
#define ENGINE_ENTITYMANAGER_H

#include <glm/glm.hpp>

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

#endif //ENGINE_ENTITYMANAGER_H