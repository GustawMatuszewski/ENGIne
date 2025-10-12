#ifndef TEXTURE_HPP
#define TEXTURE_HPP

#include <GL/glew.h>
#include <string>

#include "stb_image.h"
#include "CommonValues.hpp"

class Texture{
    public:
        Texture();
        Texture(const char* fileLocation);

        GLuint GetTextureID() const { return textureID; }
        const char* GetFileLocation() const { return fileLocation; }

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