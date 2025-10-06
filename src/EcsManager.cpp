#include "EcsManager.hpp"

int ECSManager::CreateEntity() {
    entities.push_back(nextEntityID++);
    return entities.back();
}

void ECSManager::RemoveEntity(int entityID) {
    for (size_t i = 0; i < entities.size(); ++i) {
        if (entities[i] == entityID) {
            std::swap(entities[i], entities.back());
            entities.pop_back();
            break; 
        }
    }

    transformComponents.erase(entityID);
    meshComponents.erase(entityID);
    modelComponents.erase(entityID);
    textureComponents.erase(entityID);
    materialComponents.erase(entityID);
    lightComponents.erase(entityID);
    cameraComponents.erase(entityID);
    scriptComponents.erase(entityID);
}

void ECSManager::AddTransformComponent(int entityID, const TransformComponent& component) {
    if(HasEntity(entityID))
        transformComponents[entityID] = component;
}

void ECSManager::AddMeshComponent(int entityID, const MeshComponent& component) {
    if(HasEntity(entityID))
        meshComponents[entityID] = component;
}

void ECSManager::AddModelComponent(int entityID, const ModelComponent& component) {
    if(HasEntity(entityID))
        modelComponents[entityID] = component;
}

void ECSManager::AddTextureComponent(int entityID, const TextureComponent& component) {
    if(HasEntity(entityID))
        textureComponents[entityID] = component;
}

void ECSManager::AddMaterialComponent(int entityID, const MaterialComponent& component) {
    if(HasEntity(entityID))    
        materialComponents[entityID] = component;
}

void ECSManager::AddLightComponent(int entityID, const LightComponent& component) {
    if(HasEntity(entityID))
        lightComponents[entityID] = component;
}

void ECSManager::AddCameraComponent(int entityID, const CameraComponent& component) {
    if(HasEntity(entityID))
        cameraComponents[entityID] = component;
}

void ECSManager::AddScriptComponent(int entityID, const ScriptComponent& component) {
    if(HasEntity(entityID))
        scriptComponents[entityID] = component;
}

void ECSManager::AddDirectionalLightComponent(int entityID, const DirectionalLightComponent& component) {
    if (HasEntity(entityID))
        directionalLightComponents[entityID] = component;
}

void ECSManager::AddSpotLightComponent(int entityID, const SpotLightComponent& component) {
    if (HasEntity(entityID))
        spotLightComponents[entityID] = component;
}

void ECSManager::AddPointLightComponent(int entityID, const PointLightComponent& component) {
    if (HasEntity(entityID))
        pointLightComponents[entityID] = component;
}



TransformComponent& ECSManager::GetTransformComponent(int entityID) {
    return transformComponents.at(entityID);
}

MeshComponent& ECSManager::GetMeshComponent(int entityID) {
    return meshComponents.at(entityID);
}

ModelComponent& ECSManager::GetModelComponent(int entityID) {
    return modelComponents.at(entityID);
}

TextureComponent& ECSManager::GetTextureComponent(int entityID) {
    return textureComponents.at(entityID);
}

MaterialComponent& ECSManager::GetMaterialComponent(int entityID) {
    return materialComponents.at(entityID);
}

LightComponent& ECSManager::GetLightComponent(int entityID) {
    return lightComponents.at(entityID);
}

CameraComponent& ECSManager::GetCameraComponent(int entityID) {
    return cameraComponents.at(entityID);
}

ScriptComponent& ECSManager::GetScriptComponent(int entityID) {
    return scriptComponents.at(entityID);
}

DirectionalLightComponent& ECSManager::GetDirectionalLightComponent(int entityID) {
    return directionalLightComponents.at(entityID);
}

SpotLightComponent& ECSManager::GetSpotLightComponent(int entityID) {
    return spotLightComponents.at(entityID);
}

PointLightComponent& ECSManager::GetPointLightComponent(int entityID) {
    return pointLightComponents.at(entityID);
}