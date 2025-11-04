#include "EntityManager.h"

#include <algorithm>

#include "Entity.h"

using namespace std;

void EntityManager::AddComponent(Entity* entity, Component* component) {
    entity->components.push_back(component);

    //Component Dependencies

    bool hasTransform = false;
    bool hasModel = false;
    bool hasTexture = false;
    bool hasDirLight = false;
    bool hasPointLight = false;

    if (dynamic_cast<ModelComponent*>(component)) {
        for (auto* c : entity->components) {
            if (dynamic_cast<TransformComponent*>(c)) {
                hasTransform = true;
                break;
            } else entity->components.push_back(new TransformComponent);

            if (dynamic_cast<TextureComponent*>(c)) {
                hasTexture = true;
                break;
            } else entity->components.push_back(new TextureComponent);
        }
    }
}

void EntityManager::RemoveComponent(Entity* entity, Component* component) {
    if (find(entity->components.begin(), entity->components.end(), component) != entity->components.end()) {
        entity->components.erase(find(entity->components.begin(), entity->components.end(), component));
    }
}
