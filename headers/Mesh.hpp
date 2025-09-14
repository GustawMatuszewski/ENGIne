#ifndef MESH_HPP
#define MESH_HPP

#include <GL/glew.h>

class Mesh{
    public:
        Mesh();

        void CreateMesh(GLfloat *vertices, unsigned int *indices, unsigned int numOfVertices, unsigned int numOfIndices);
        void RenderMesh();
        void ClearMesh();

        ~Mesh();
    private:
        GLuint VAO, VBO, EBO;
        GLsizei indexCount;
};

#endif