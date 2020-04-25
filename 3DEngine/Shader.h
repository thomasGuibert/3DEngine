#pragma once
#include <glad/glad.h>
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Shader
{
public:
    Shader(std::string vertexShaderPath, std::string fragmentShaderPath) {
        std::string vsCode = readShaderFile(vertexShaderPath);
        int vsId = CompileShader(vsCode.c_str(), GL_VERTEX_SHADER);
        std::string fsCode = readShaderFile(fragmentShaderPath);
        int fsId = CompileShader(fsCode.c_str(), GL_FRAGMENT_SHADER);

        _shaderProgramId = CreateShaderProgram(vsId, fsId);
        glDeleteShader(vsId);
        glDeleteShader(fsId);
    };

    void UpdateUniformMat4(const GLchar* name, glm::mat4 &value) {
        glUseProgram(_shaderProgramId);
        unsigned int uniformId = glGetUniformLocation(_shaderProgramId, name);
        glUniformMatrix4fv(uniformId, 1, GL_FALSE, glm::value_ptr(value));
    };

    unsigned int Id() {
        return _shaderProgramId;
    };
    ~Shader();
private:
    unsigned int _shaderProgramId;

    std::string readShaderFile(std::string shaderPath) {
        std::string shaderCode;
        std::ifstream shaderFileReader;
        shaderFileReader.exceptions(std::ifstream::failbit | std::ifstream::badbit);
        try {
            shaderFileReader.open(shaderPath);
            std::stringstream shaderFileStream;
            shaderFileStream << shaderFileReader.rdbuf();
            shaderFileReader.close();
            shaderCode = shaderFileStream.str();
        }
        catch (std::ifstream::failure& e)
        {
            std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
        }
        return shaderCode;
    };
    int CompileShader(const char *shaderSource, GLenum shaderType) {
        int shaderId = glCreateShader(shaderType);
        glShaderSource(shaderId, 1, &shaderSource, NULL);
        glCompileShader(shaderId);

        // check for shader compile errors
        int success;
        char infoLog[512];
        glGetShaderiv(shaderId, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            glGetShaderInfoLog(shaderId, 512, NULL, infoLog);
            std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
        }

        return shaderId;
    };

    int CreateShaderProgram(int vertexShaderId, int fragmentShaderId) {
        int shaderProgramId = glCreateProgram();
        glAttachShader(shaderProgramId, vertexShaderId);
        glAttachShader(shaderProgramId, fragmentShaderId);
        glLinkProgram(shaderProgramId);

        // check for linking errors
        int success;
        char infoLog[512];
        glGetProgramiv(shaderProgramId, GL_LINK_STATUS, &success);
        if (!success) {
            glGetProgramInfoLog(shaderProgramId, 512, NULL, infoLog);
            std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
        }

        return shaderProgramId;
    }
};

