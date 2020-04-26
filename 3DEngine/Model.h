#pragma once
#include <string>
#include <Shader.h>
#include <ImageTexture.h>

class Model
{
private:
    unsigned int _VBO, _VAO;
    glm::vec3 _scale = glm::vec3(1.0);
    Shader _shader;

public:
    Model(const float vertices[], unsigned int bufferSize, Shader shader);
    void drawOnPositions(const glm::vec3 cubePositions[], const int size);
    void setScale(const glm::vec3 scale);
};

