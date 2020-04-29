#pragma once
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <string>
#include <fstream>
#include <sstream>
#include <glad/glad.h>

class Shader
{
public:
    Shader(std::string vertexShaderPath, std::string fragmentShaderPath);
    void updateUniformMat4(const GLchar *name, glm::mat4 &value);
    void updateUniformVec3(const GLchar *name, glm::vec3 &value);
    void updateUniformFloat(const GLchar *name, float value);
    void updateUniformInt(const GLchar* name, int value);

private:
    unsigned int _shaderProgramId;

    std::string _readShaderFile(std::string shaderPath);
    int _compileShader(const char *shaderSource, GLenum shaderType);
    int _createShaderProgram(int vertexShaderId, int fragmentShaderId);
};

