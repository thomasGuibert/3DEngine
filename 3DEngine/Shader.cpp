#include "Shader.h"

Shader::Shader(std::string vertexShaderPath, std::string fragmentShaderPath) {
    std::string vsCode = _readShaderFile(vertexShaderPath);
    int vsId = _compileShader(vsCode.c_str(), GL_VERTEX_SHADER);
    std::string fsCode = _readShaderFile(fragmentShaderPath);
    int fsId = _compileShader(fsCode.c_str(), GL_FRAGMENT_SHADER);

    shaderProgramId = _createShaderProgram(vsId, fsId);
    glDeleteShader(vsId);
    glDeleteShader(fsId);
};

void Shader::updateUniformMat4(const std::string name, glm::mat4 &value) {
    glUseProgram(shaderProgramId);
    unsigned int uniformId = glGetUniformLocation(shaderProgramId, name.c_str());
    glUniformMatrix4fv(uniformId, 1, GL_FALSE, glm::value_ptr(value));
};

void Shader::updateUniformVec3(const std::string name, glm::vec3 &value) {
    glUseProgram(shaderProgramId);
    unsigned int uniformId = glGetUniformLocation(shaderProgramId, name.c_str());
    glUniform3fv(uniformId, 1, glm::value_ptr(value));
};

void Shader::updateUniformVec3(const std::string name, float x, float y, float z) {
    glm::vec3 value(x, y, z);
    updateUniformVec3(name, value);
};

void Shader::updateUniformFloat(const std::string name, float value) {
    glUseProgram(shaderProgramId);
    unsigned int uniformId = glGetUniformLocation(shaderProgramId, name.c_str());
    glUniform1fv(uniformId, 1, &value);
};

void Shader::updateUniformInt(const std::string name, int value) {
    glUseProgram(shaderProgramId);
    unsigned int uniformId = glGetUniformLocation(shaderProgramId, name.c_str());
    glUniform1iv(uniformId, 1, &value);
};

std::string Shader::_readShaderFile(std::string shaderPath) {
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
        std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ: "<< shaderPath << std::endl;
    }
    return shaderCode;
};
int Shader::_compileShader(const char *shaderSource, GLenum shaderType) {
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

int Shader::_createShaderProgram(int vertexShaderId, int fragmentShaderId) {
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
