#include "Shader.hpp"

Shader::Shader(){
    shaderID = 0;
    uniformModel = 0;
    uniformProjection = 0;

    pointLightCount = 0;
    spotLightCount = 0;
}

void Shader::CreateFromString(const char* vertCode, const char* fragCode){
    CompileShader(vertCode, fragCode);
}

void Shader::CreateFromFiles(const char* vertLocation, const char* fragLocation){
    std::string vertString = ReadFile(vertLocation);
    std::string fragString = ReadFile(fragLocation);

    const char* vertCode = vertString.c_str();
    const char* fragCode = fragString.c_str();

    CompileShader(vertCode, fragCode);
}

std::string Shader::ReadFile(const char* fileLocation){
    std::string content;
    std::ifstream fileStream(fileLocation, std::ios::in);

    if(!fileStream.is_open()){
        printf("ERROR: FAILED TO READ SHADER !!! \n");
        return "";
    }

    std::string line ="";
    while(!fileStream.eof()){
        std::getline(fileStream, line);
        content.append(line+"\n");
    }

    fileStream.close();
    return content;
}

void Shader::CompileShader(const char* vertCode, const char* fragCode){
    shaderID = glCreateProgram();
    if(!shaderID){
        printf("ERROR: FAILED TO CREATE SHADER PROGRAM !!! \n");
        return;
    }

    AddShader(shaderID, vertCode, GL_VERTEX_SHADER);
    AddShader(shaderID, fragCode, GL_FRAGMENT_SHADER);

    GLint result = 0;
    GLchar eLog[1024] = {0};

    glLinkProgram(shaderID);
    glGetProgramiv(shaderID, GL_LINK_STATUS, &result);
    if(!result){
        glGetProgramInfoLog(shaderID, sizeof(eLog), NULL, eLog);
        printf("ERROR LINKING PROGRAM: '%s' !!! \n ", eLog);
        return;
    }

    glValidateProgram(shaderID);
    glGetProgramiv(shaderID, GL_VALIDATE_STATUS, &result);
    if(!result){
        glGetProgramInfoLog(shaderID, sizeof(eLog), NULL, eLog);
        printf("ERROR VALIDATING PROGRAM: '%s' !!! \n", eLog);
        return;
    }

    uniformProjection = glGetUniformLocation(shaderID, "projection");
    uniformModel = glGetUniformLocation(shaderID, "model");
    uniformView = glGetUniformLocation(shaderID, "view");
    uniformDirectionalLight.uniformColor = glGetUniformLocation(shaderID, "directionalLight.base.color");
    uniformDirectionalLight.uniformAmbientIntensity = glGetUniformLocation(shaderID, "directionalLight.base.ambientIntensity");
    uniformDirectionalLight.uniformDirection = glGetUniformLocation(shaderID, "directionalLight.direction");
    uniformDirectionalLight.uniformDiffuseIntensity = glGetUniformLocation(shaderID, "directionalLight.base.diffuseIntensity");
    uniformSpecularIntensity = glGetUniformLocation(shaderID, "material.specularIntensity");
    uniformShininess = glGetUniformLocation(shaderID, "material.shininess");
    uniformEyePos = glGetUniformLocation(shaderID, "eyePos");

    uniformPointLightCount = glGetUniformLocation(shaderID, "pointLightCount");

    for(size_t i = 0; i < MAX_POINT_LIGHTS; i++){
        char locBuff[100] = {'\0'};

        snprintf(locBuff, sizeof(locBuff), "pointLights[%d].base.color", i);
        uniformPointLight[i].uniformColor = glGetUniformLocation(shaderID, locBuff);

        snprintf(locBuff, sizeof(locBuff), "pointLights[%d].base.ambientIntensity", i);
        uniformPointLight[i].uniformAmbientIntensity = glGetUniformLocation(shaderID, locBuff);

        snprintf(locBuff, sizeof(locBuff), "pointLights[%d].base.diffuseIntensity", i);
        uniformPointLight[i].uniformDiffuseIntensity = glGetUniformLocation(shaderID, locBuff);

        snprintf(locBuff, sizeof(locBuff), "pointLights[%d].position", i);
        uniformPointLight[i].uniformPosition = glGetUniformLocation(shaderID, locBuff);

        snprintf(locBuff, sizeof(locBuff), "pointLights[%d].constant", i);
        uniformPointLight[i].uniformConstant = glGetUniformLocation(shaderID, locBuff);

        snprintf(locBuff, sizeof(locBuff), "pointLights[%d].linear", i);
        uniformPointLight[i].uniformLinear= glGetUniformLocation(shaderID, locBuff);

        snprintf(locBuff, sizeof(locBuff), "pointLights[%d].exponent", i);
        uniformPointLight[i].uniformExponent = glGetUniformLocation(shaderID, locBuff);
    }

    uniformSpotLightCount = glGetUniformLocation(shaderID, "spotLightCount");

    for(size_t i = 0; i < MAX_SPOT_LIGHTS; i++){
        char locBuff[100] = {'\0'};

        snprintf(locBuff, sizeof(locBuff), "spotLights[%d].base.base.color", i);
        uniformSpotLight[i].uniformColor = glGetUniformLocation(shaderID, locBuff);

        snprintf(locBuff, sizeof(locBuff), "spotLights[%d].base.base.ambientIntensity", i);
        uniformSpotLight[i].uniformAmbientIntensity = glGetUniformLocation(shaderID, locBuff);

        snprintf(locBuff, sizeof(locBuff), "spotLights[%d].base.base.diffuseIntensity", i);
        uniformSpotLight[i].uniformDiffuseIntensity = glGetUniformLocation(shaderID, locBuff);

        snprintf(locBuff, sizeof(locBuff), "spotLights[%d].base.position", i);
        uniformSpotLight[i].uniformPosition = glGetUniformLocation(shaderID, locBuff);

        snprintf(locBuff, sizeof(locBuff), "spotLights[%d].base.constant", i);
        uniformSpotLight[i].uniformConstant = glGetUniformLocation(shaderID, locBuff);

        snprintf(locBuff, sizeof(locBuff), "spotLights[%d].base.linear", i);
        uniformSpotLight[i].uniformLinear= glGetUniformLocation(shaderID, locBuff);

        snprintf(locBuff, sizeof(locBuff), "spotLights[%d].base.exponent", i);
        uniformSpotLight[i].uniformExponent = glGetUniformLocation(shaderID, locBuff);

        snprintf(locBuff, sizeof(locBuff), "spotLights[%d].direction", i);
        uniformSpotLight[i].uniformDirection = glGetUniformLocation(shaderID, locBuff);

        snprintf(locBuff, sizeof(locBuff), "spotLights[%d].edge", i);
        uniformSpotLight[i].uniformEdge = glGetUniformLocation(shaderID, locBuff);
    }
}

GLuint Shader::GetProjectionLocation(){
    return uniformProjection;
}

GLuint Shader::GetModelLocation(){
    return uniformModel;
}

GLuint Shader::GetViewLocation(){
    return uniformView;
}

GLuint Shader::GetAmbientColorLocation(){
    return uniformDirectionalLight.uniformColor;
}

GLuint Shader::GetAmbientIntensityLocation(){
    return uniformDirectionalLight.uniformAmbientIntensity;
}

GLuint Shader::GetDiffuseIntensityLocation(){
    return uniformDirectionalLight.uniformDiffuseIntensity;
}

GLuint Shader::GetDirectionLocation(){
    return uniformDirectionalLight.uniformDirection;
}

GLuint Shader::GetSpecularIntensityLocation(){
    return uniformSpecularIntensity;
}

GLuint Shader::GetShininessLocation(){
    return uniformShininess;
}

GLuint Shader::GetEyePosLocation(){
    return uniformEyePos;
}

void Shader::SetDirectionalLight(DirectionalLight *dLight){
    dLight->UseLight(uniformDirectionalLight.uniformAmbientIntensity, uniformDirectionalLight.uniformColor,
                        uniformDirectionalLight.uniformDiffuseIntensity, uniformDirectionalLight.uniformDirection);
}

void Shader::SetPointLights(PointLight *pLight, unsigned int lightCount){
    if(lightCount > MAX_POINT_LIGHTS) lightCount = MAX_POINT_LIGHTS;

    glUniform1i(uniformPointLightCount, lightCount);

    for(size_t i = 0; i < lightCount; i++){
        pLight[i].UseLight(uniformPointLight[i].uniformAmbientIntensity, uniformPointLight[i].uniformColor,
            uniformPointLight[i].uniformDiffuseIntensity, uniformPointLight[i].uniformPosition,
            uniformPointLight[i].uniformConstant, uniformPointLight[i].uniformLinear, uniformPointLight[i].uniformExponent
        );
    }
}

void Shader::SetSpotLights(SpotLight *sLight, unsigned int lightCount){
    if(lightCount > MAX_SPOT_LIGHTS) lightCount = MAX_SPOT_LIGHTS;

    glUniform1i(uniformSpotLightCount, lightCount);

    for(size_t i = 0; i < lightCount; i++){
        sLight[i].UseLight(uniformSpotLight[i].uniformAmbientIntensity, uniformSpotLight[i].uniformColor,
            uniformSpotLight[i].uniformDiffuseIntensity, uniformSpotLight[i].uniformPosition, uniformSpotLight[i].uniformDirection,
            uniformSpotLight[i].uniformConstant, uniformSpotLight[i].uniformLinear, uniformSpotLight[i].uniformExponent, uniformSpotLight[i].uniformEdge
        );
    }
}

void Shader::UseShader(){
    glUseProgram(shaderID);
}

void Shader::ClearShader(){
    if(shaderID != 0){
        glDeleteProgram(shaderID);
        shaderID = 0;

        uniformModel = 0;
        uniformProjection = 0;
    }
}

void Shader::AddShader(GLuint Program, const char* shaderCode, GLenum shaderType){
    GLuint Shader = glCreateShader(shaderType);

    const GLchar* theCode[1];
    theCode[0] = shaderCode;

    GLint codeLength[1];
    codeLength[0] = strlen(shaderCode);

    glShaderSource(Shader, 1, theCode, codeLength);
    glCompileShader(Shader);

    GLint result = 0;
    GLchar eLog[1024] = {0};

    glGetShaderiv(Shader, GL_COMPILE_STATUS, &result);
    if(!result){
        glGetShaderInfoLog(Shader, sizeof(eLog), NULL, eLog);
        printf("ERROR: COMPILING THE %d SHADER FAILED !!! '%s' \n ", shaderType, eLog);
        return;
    }

    glAttachShader(Program, Shader);
    
}

Shader::~Shader(){
    ClearShader();
}