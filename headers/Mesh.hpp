#ifndef MESH_HPP
#define MESH_HPP

#include <vector>

#include <GL/glew.h>

class Mesh{
    public:
        Mesh();

        void CreateMesh(const GLfloat *vertices, const unsigned int *indices, unsigned int numOfVertices, unsigned int numOfIndices);
        void RenderMesh();
        void ClearMesh();

        void SetData(const std::vector<GLfloat>& verts, const std::vector<unsigned int>& inds) {
            verticesCPU = verts;
            indicesCPU = inds;
        }

        const std::vector<GLfloat>& GetVertices() const { return verticesCPU; }
        const std::vector<unsigned int>& GetIndices() const { return indicesCPU; }

        ~Mesh();
    private:
        GLuint VAO, VBO, EBO;
        GLsizei indexCount;

        std::vector<GLfloat> verticesCPU;
        std::vector<unsigned int> indicesCPU;
};

#endif