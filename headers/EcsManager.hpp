#ifndef ECSMANAGER_HPP
#define ECSMANAGER_HPP

#include <vector>
#include <unordered_map>

#include "Entity.hpp"

class ECSManager {
    public:
        int CreateEntity();
        std::vector<int > GetEntities() const { return entities; }
        void RemoveEntity(int entityID);

        bool HasEntity(int entityID) {
            return std::find(entities.begin(), entities.end(), entityID) != entities.end();
        }

        template<typename T>
        bool HasComponent(int entityID) const {
            if constexpr (std::is_same_v<T, TransformComponent>) {
                return transformComponents.count(entityID) > 0;
            } else if constexpr (std::is_same_v<T, MeshComponent>) {
                return meshComponents.count(entityID) > 0;
            } else if constexpr (std::is_same_v<T, ModelComponent>) {
                return modelComponents.count(entityID) > 0;
            } else if constexpr (std::is_same_v<T, TextureComponent>) {
                return textureComponents.count(entityID) > 0;
            } else if constexpr (std::is_same_v<T, MaterialComponent>) {
                return materialComponents.count(entityID) > 0;
            } else if constexpr (std::is_same_v<T, CameraComponent>) {
                return cameraComponents.count(entityID) > 0;
            } else if constexpr (std::is_same_v<T, ScriptComponent>) {
                return scriptComponents.count(entityID) > 0;
            } else if constexpr (std::is_same_v<T, DirectionalLightComponent>) {
                return directionalLightComponents.count(entityID) > 0;
            } else if constexpr (std::is_same_v<T, SpotLightComponent>) {
                return spotLightComponents.count(entityID) > 0;
            } else if constexpr (std::is_same_v<T, PointLightComponent>) {
                return pointLightComponents.count(entityID) > 0;
            }
            
            return false;
        }

        void AddTransformComponent(int entityID, const TransformComponent& component);
        void AddMeshComponent(int entityID, const MeshComponent& component);
        void AddModelComponent(int entityID, const ModelComponent& component);
        void AddTextureComponent(int entityID, const TextureComponent& component);
        void AddMaterialComponent(int entityID, const MaterialComponent& component);
        void AddCameraComponent(int entityID, const CameraComponent& component);
        void AddScriptComponent(int entityID, const ScriptComponent& component);
        void AddLightComponent(int entityID, const LightComponent& component);
        void AddDirectionalLightComponent(int entityID, const DirectionalLightComponent& component);
        void AddSpotLightComponent(int entityID, const SpotLightComponent& component);
        void AddPointLightComponent(int entityID, const PointLightComponent& component);

        TransformComponent& GetTransformComponent(int entityID);
        MeshComponent& GetMeshComponent(int entityID);
        ModelComponent& GetModelComponent(int entityID);
        TextureComponent& GetTextureComponent(int entityID);
        MaterialComponent& GetMaterialComponent(int entityID);
        CameraComponent& GetCameraComponent(int entityID);
        ScriptComponent& GetScriptComponent(int entityID);
        LightComponent& GetLightComponent(int entityID);
        DirectionalLightComponent& GetDirectionalLightComponent(int entityID);
        SpotLightComponent& GetSpotLightComponent(int entityID);
        PointLightComponent& GetPointLightComponent(int entityID);
        

    private:
        std::vector<int> entities;
        int nextEntityID = 0;

        std::unordered_map<int, TransformComponent> transformComponents;
        std::unordered_map<int, MeshComponent> meshComponents;
        std::unordered_map<int, ModelComponent> modelComponents;
        std::unordered_map<int, TextureComponent> textureComponents;
        std::unordered_map<int, MaterialComponent> materialComponents;
        std::unordered_map<int, LightComponent> lightComponents;
        std::unordered_map<int, CameraComponent> cameraComponents;
        std::unordered_map<int, ScriptComponent> scriptComponents;
        std::unordered_map<int, DirectionalLightComponent> directionalLightComponents;
        std::unordered_map<int, SpotLightComponent> spotLightComponents;
        std::unordered_map<int, PointLightComponent> pointLightComponents;
};
#endif