#pragma once
#include "Shader.h"
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

struct Face {
    float FRONT_FACE[18] = { 1, 1, 1,  0, 1, 1,  0, 0, 1,  0, 0, 1,  1, 0, 1,  1, 1, 1 };
    float LEFT_FACE[18] = { 0, 1, 1,  0, 1, 0,  0, 0, 0,  0, 0, 0,  0, 0, 1,  0, 1, 1 };
    float BACK_FACE[18] = { 0, 1, 0,  1, 1, 0,  1, 0, 0,  1, 0, 0,  0, 0, 0,  0, 1, 0 };
    float RIGHT_FACE[18] = { 1, 1, 0,  1, 1, 1,  1, 0, 1,  1, 0, 1,  1, 0, 0,  1, 1, 0 };
    float TOP_FACE[18] = { 1, 1, 0,  0, 1, 0,  0, 1, 1,  0, 1, 1,  1, 1, 1,  1, 1, 0 };
    float BOTTOM_FACE[18] = { 0, 0, 0,  1, 0, 0,  1, 0, 1,  1, 0, 1,  0, 0, 1,  0, 0, 0 };
};
const Face FACE;

struct TextureCoords {
    const std::vector<float> COORDS = { 0, 0,  1, 0,  1, 1,  1 ,1,  0, 1,  0, 0 };
};
const TextureCoords TEXTURE;

struct Normals {
    float FRONT_FACE[18] = { 0,  0, -1,    0,  0, -1,    0,  0, -1,    0,  0, -1,    0,  0, -1,    0,  0, -1 };
    float BACK_FACE[18] = { 0,  0,  1,    0,  0,  1,    0,  0,  1,    0,  0,  1,    0,  0,  1,    0,  0,  1 };
    float LEFT_FACE[18] = { -1,  0,  0,   -1,  0,  0,   -1,  0,  0,   -1,  0,  0,   -1,  0,  0,   -1,  0,  0 };
    float RIGHT_FACE[18] = { 1,  0,  0,    1,  0,  0,    1,  0,  0,    1,  0,  0,    1,  0,  0,    1,  0,  0 };
    float BOTTOM_FACE[18] = { 0, -1,  0,    0, -1,  0,    0, -1,  0,    0, -1,  0,    0, -1,  0,    0, -1,  0 };
    float TOP_FACE[18] = { 0,  1,  0,    0,  1,  0,    0,  1,  0,    0,  1,  0,    0,  1,  0,    0,  1,  0 };
};
const Normals NORMALS;

class GL_Block
{
public:
    void addFace(const float* faceVertices, const float* faceNormal, int type);
    void addHighlight(const float* faceVertices);
    void build();
    void buildHighlight();
    void render(Shader& shader, Shader& shaderHighlight, glm::vec3 position);
    void translate(const float vertices[], float outVertices[], glm::vec3 offset);
    ~GL_Block();
private:
    unsigned int _VAO, _VAOHighlight;
    std::vector<float> _vertices;
    std::vector<float> _verticesHighlight;

};

