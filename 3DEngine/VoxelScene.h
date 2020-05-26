#pragma once
#include "Scene.h"
#include "Skybox.h"
#include "CameraFPS.h"
#include "ChunkManager.h"
#include "ImageTexture.h"

class VoxelScene :
    public Scene
{
public:
    Skybox* skybox;

    VoxelScene(CameraFPS& camera);
    
    void render();
    
    ~VoxelScene();

private:
    Shader _voxelShader;
    Shader _voxelshaderHighlight;
    ImageTexture* _texture;
    ChunkManager* chunkManager;

};

