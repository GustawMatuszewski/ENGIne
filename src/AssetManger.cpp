#include "AssetManager.h"

std::shared_ptr<Model> AssetManager::LoadModel(const std::string& name, const std::string &path) {
    if (models.contains(name))
        return models[name];

    auto model = std::make_shared<Model>();
    model->LoadModel(path);
    models[name] = model;
    return model;
}

std::shared_ptr<Texture> AssetManager::LoadTexture(const std::string& name, const std::string &path, bool alfa) {
    if (textures.contains(name))
        return textures[name];

    auto texture = std::make_shared<Texture>();
    alfa ? texture->LoadTexture2D_A() : texture->LoadTexture2D();
    textures[name] = texture;
    return texture;
}

std::shared_ptr<Shader> AssetManager::LoadShader(const std::string& name, const char*& vert, const char*& frag) {
    if (shaders.contains(name))
        return shaders[name];

    auto shader = std::make_shared<Shader>();
    shader->CreateFromFiles(vert, frag);
    shaders[name] = shader;
    return shader;

}

std::shared_ptr<Material> AssetManager::CreateMaterial(const std::string& name, float specularIntensity, float shininess) {
    if (materials.contains(name))
        return materials[name];

    auto material = std::make_shared<Material>(specularIntensity, shininess);
    materials[name] = material;
    return material;
}


void AssetManager::Clear() {
    textures.clear();
    models.clear();
    shaders.clear();
    materials.clear();
}