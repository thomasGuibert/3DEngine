#pragma once
#include "BaseCameraBehavior.h"
#include "ChunkManager.h"

class VoxelCameraBehavior : public BaseCameraBehavior
{
public:
    VoxelCameraBehavior(Camera& camera);
    ~VoxelCameraBehavior();
    virtual void processKeyboardDirection(Direction direction, float deltaTime);
    virtual void processKeyboardAction(Action action);

    void setChunkManager(ChunkManager* chunckManager);
private:
    ChunkManager* _chunkManager;
};

