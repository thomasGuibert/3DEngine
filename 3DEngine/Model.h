#pragma once
#include <string>
#include <Shader.h>
#include <ImageTexture.h>

class Model
{
private:
    unsigned int _VBO, _VAO;
    Shader _shader;

public:
    Model(const float vertices[], unsigned int bufferSize, Shader shader);
    void DrawOnPositions(const glm::vec3 cubePositions[]);
};

