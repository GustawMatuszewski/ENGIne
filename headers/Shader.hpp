#ifndef SHADER_HPP
#define SHADER_HPP

#include <stdio.h>
#include <string.h>
#include <iostream>
#include <fstream>

#include <GL/glew.h>

#include "CommonValues.hpp"

#include "DirectionalLight.hpp"
#include "PointLight.hpp"
#include "SpotLight.hpp"

class Shader{
    public:
        Shader();

        void CreateFromString(const char* vertCode, const char* fragCode);
        void CreateFromFiles(const char* vertLocation, const char* fragLocation);

        std::string ReadFile(const char* fileLocation);

        GLuint GetProjectionLocation();
        GLuint GetModelLocation();
        GLuint GetViewLocation();
        GLuint GetAmbientIntensityLocation();
        GLuint GetAmbientColorLocation();
        GLuint GetDiffuseIntensityLocation();
        GLuint GetDirectionLocation();
        GLuint GetSpecularIntensityLocation();
        GLuint GetShininessLocation();
        GLuint GetEyePosLocation();

        void SetDirectionalLight(DirectionalLight *dLight);
        void SetPointLights(PointLight *pLight, unsigned int lightCount);
        void SetSpotLights(SpotLight *sLight, unsigned int lightCount);

        void UseShader();
        void ClearShader();

        ~Shader();

    private:
        int pointLightCount;
        int spotLightCount;

        GLuint shaderID, uniformModel, uniformProjection,
        uniformView, uniformAmbientIntensity, uniformAmbientColor,
        uniformDiffuseIntensity, uniformDirection, uniformEyePos,
        uniformSpecularIntensity, uniformShininess;

        struct{
            GLuint uniformColor;
            GLuint uniformAmbientIntensity;
            GLuint uniformDiffuseIntensity;

            GLuint uniformDirection;
        } uniformDirectionalLight;

        GLuint uniformPointLightCount;

        struct{
            GLuint uniformColor;
            GLuint uniformAmbientIntensity;
            GLuint uniformDiffuseIntensity;

            GLuint uniformPosition;
            GLuint uniformConstant;
            GLuint uniformLinear;
            GLuint uniformExponent;
        } uniformPointLight[MAX_POINT_LIGHTS];

        GLuint uniformSpotLightCount;

        struct {
            GLuint uniformColor;
            GLuint uniformAmbientIntensity;
            GLuint uniformDiffuseIntensity;

            GLuint uniformPosition;
            GLuint uniformConstant;
            GLuint uniformLinear;
            GLuint uniformExponent;

            GLuint uniformDirection;
            GLuint uniformEdge;
        } uniformSpotLight[MAX_SPOT_LIGHTS];

        void CompileShader(const char* vertCode, const char* fragCode);
        void AddShader(GLuint Program, const char* shaderCode, GLenum shaderType);

};

#endif