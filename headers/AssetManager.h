#pragma once

#include <memory>
#include <unordered_map>
#include <string>

#include <glm/glm.hpp>

#include "Texture.hpp"
#include "Model.hpp"
#include "Shader.hpp"
#include "Material.hpp"

class AssetManager {
    public:
        std::shared_ptr<Texture> LoadTexture(const std::string& name, const std::string& path, bool alfa);
        std::shared_ptr<Model> LoadModel(const std::string& name, const std::string& path);
        std::shared_ptr<Shader> LoadShader(const std::string& name, const char*& vert, const char*& frag);
        std::shared_ptr<Material> CreateMaterial(const std::string& name, GLfloat  specularIntensity, GLfloat  shininess);

        void Clear();


        std::unordered_map<std::string, std::shared_ptr<Texture>> textures;
        std::unordered_map<std::string, std::shared_ptr<Model>> models;
        std::unordered_map<std::string, std::shared_ptr<Shader>> shaders;
        std::unordered_map<std::string, std::shared_ptr<Material>> materials;
};
