#pragma once
#include "Camera.h"
#include "ChunkManager.h"

class CameraFPS: public Camera
{
public:
    CameraFPS();
    void setChunkManager(ChunkManager* chunckManager);
    void processKeyboardDirection(Direction direction, float deltaTime);
    void processKeyboardAction(Action action);
    ~CameraFPS();
private:
    ChunkManager* _chunkManager;
};

