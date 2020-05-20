#pragma once
#include "Block.h"
#include "GL_Block.h"
#include <glm/ext.hpp>

class Chunk
{
public:
    Chunk (Shader& shader, glm::vec3 position);
    ~Chunk();

    void CreateMesh();

    void CreateCube(GL_Block& block, int x, int y, int z, glm::vec3 offset);

    //void Update(float dt);

    void Setup_Sphere();

    void Setup_Landscape();

    void Render();

    static const int CHUNK_SIZE = 16;

    GL_Block* blockRenderer;
private:
    // The blocks data
    Block*** m_pBlocks;
    Shader _shader;
    glm::vec3 _position;
};

