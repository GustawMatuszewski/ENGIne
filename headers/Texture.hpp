#ifndef TEXTURE_HPP
#define TEXTURE_HPP

#include <GL/glew.h>

#include "stb_image.h"

class Texture{
    public:
        Texture();
        Texture(const char* fileLocation);

        GLuint GetTextureID() const { return textureID; }

        bool LoadTexture2D();
        bool LoadTexture2D_A();
        
        void UseTexture();
        void ClearTexture();

        ~Texture();
    private:
        GLuint textureID;
        int width, height, bitDepth;

        const char* fileLocation;
};

#endif