#ifndef ENGINE_ENTITY_H
#define ENGINE_ENTITY_H

#include <string>
#include <vector>

//class EntityManager;
class Component;

class Entity {
    public:
        int entityID;
        std::string name;
        std::vector<Entity*> children;
        std::vector<Component*> components;
};

#endif