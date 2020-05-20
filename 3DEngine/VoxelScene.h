#pragma once
#include "Scene.h"
#include "ChunkManager.h"
#include "ImageTexture.h"

class VoxelScene :
    public Scene
{
public:
    VoxelScene(Camera& camera);
    
    void render();
    
    ~VoxelScene();

private:
    Shader _voxelShader;
    ImageTexture* _texture;
    ChunkManager* chunkManager;
};

