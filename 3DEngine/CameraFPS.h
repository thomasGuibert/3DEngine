#pragma once
#include "Camera.h"
#include "ChunkManager.h"

class CameraFPS: public Camera
{
public:
    CameraFPS();
    void setChunkManager(ChunkManager* chunckManager);
    void processKeyboard(Direction direction, float deltaTime);
    ~CameraFPS();
private:
    ChunkManager* _chunkManager;
};

