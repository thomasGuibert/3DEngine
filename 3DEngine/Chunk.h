#pragma once
#include "Block.h"
#include "GL_Block.h"
#include <glm/ext.hpp>

class VoxelFace {

public:
    bool transparent;
    int type;
    int side;

    bool equals(VoxelFace* face) { return face->transparent == transparent && face->type == type; }
};


class Chunk
{
public:
    Chunk(Shader& shader, Shader& shaderHighlight, glm::vec3 position);
    ~Chunk();

    void CreateMesh();

    void CreateCube(GL_Block& block, int x, int y, int z, glm::vec3 offset);

    //void Update(float dt);

    void Setup_Sphere();
    void Setup_Cube();
    void Setup_Landscape();

    void Render();

    void setHighlightedBlock(int x, int y, int z);

    void greedy();

    VoxelFace* getVoxelFace(int x, int y, int z, int side);

    void quad(glm::vec3 bottomLeft,
        glm::vec3 topLeft,
        glm::vec3 topRight,
        glm::vec3 bottomRight,
        int width,
        int height,
        VoxelFace* voxel,
        bool backFace);

    static const int CHUNK_SIZE = 16;
    Block* getBlock(const unsigned int x, const unsigned int y, const unsigned int z);
    void setHasChanged(bool hasChanged); 
    bool getHasChanged();


    GL_Block* blockRenderer;
private:
    // The blocks data
    Block*** m_pBlocks;
    Shader _shader;
    Shader _shaderHighlight;
    glm::vec3 _position;
    VoxelFace*** _voxelsFace;
    GL_Block* _blockRenderer;
    bool _hasChanged = false;
};

