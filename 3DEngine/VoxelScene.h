#pragma once
#include "Scene.h"

class VoxelScene :
    public Scene
{
public:
    VoxelScene(Camera &camera);
    
    void render();
    
    ~VoxelScene();
};

