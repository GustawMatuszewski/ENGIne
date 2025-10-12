#ifndef MODEL_HPP
#define MODEL_HPP

#include <vector>
#include <string>
#include <thread>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "Mesh.hpp"
#include "Texture.hpp"

class Model{
    public:
        Model();

        void LoadModel(const std::string &fileName);
        void RenderModel();
        void ClearModel();

        std::vector<GLuint> textureIDs;

        std::vector<Mesh*> GetMeshList() {return meshList;}

        ~Model();

    private:
        void LoadNode(aiNode *node, const aiScene *scene);
        void LoadMesh(aiMesh *mesh, const aiScene *scene);
        void LoadMaterials(const aiScene *scene);        

        std::vector<Mesh*> meshList;
        std::vector<Texture*> textureList;
        std::vector<unsigned int> meshToTex;

};

#endif