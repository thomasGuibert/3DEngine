#pragma once
#include <vector>
#include <glm/glm.hpp>
#include <ImageTexture.h>

struct Vertex {
    // position
    glm::vec3 Position;
    // normal
    glm::vec3 Normal;
    // texCoords
    glm::vec2 TexCoords;
    // tangent
    glm::vec3 Tangent;
    // bitangent
    glm::vec3 Bitangent;
};

class Geometry
{
public:
    Geometry(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<ImageTexture> textures);
    Geometry(std::vector<Vertex> vertices, std::vector<unsigned int> indices);
    ~Geometry();

    std::vector<Vertex> getVertices();
    std::vector<unsigned int> getIndices();
    std::vector<ImageTexture> getTextures();

    std::vector<Vertex>       _vertices;
    std::vector<unsigned int> _indices;
    std::vector<ImageTexture> _textures;
};

