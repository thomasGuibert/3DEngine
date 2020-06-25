#pragma once
#include "Scene.h"
#include "Skybox.h"
#include "CameraFPS.h"
#include "ChunkManager.h"
#include "ImageTexture.h"
#include "VoxelCameraBehavior.h"
#include <glm/ext.hpp>

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
    Shader _voxelShaderHighlight;
    Shader _viewfinderShader;
    ImageTexture* _texture;
    ChunkManager* chunkManager;
    unsigned int _cursorVAO;

    void createCursor();
};

