#pragma once
#include "Scene.h"
#include "Skybox.h"
#include "CameraFPS.h"
#include "ChunkManager.h"
#include "ImageTexture.h"
#include "VoxelCameraBehavior.h"

class VoxelScene :
    public Scene
{
public:
    Skybox* skybox;

    VoxelScene(VoxelCameraBehavior& manipulator);
    
    void render();
    
    ~VoxelScene();

private:
    Shader _voxelShader;
    Shader _voxelshaderHighlight;
    ImageTexture* _texture;
    ChunkManager* chunkManager;

};

