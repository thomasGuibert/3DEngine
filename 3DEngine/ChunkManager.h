#pragma once
#include "Chunk.h"

class ChunkManager
{
public:
    ChunkManager();
    ~ChunkManager();

    void Update();
    void AddChunk(Shader& shader, glm::vec3 position);
    //void Update(float dt, glm::vec3 cameraPosition, glm::vec3 cameraView);
    //void UpdateLoadList();
    //void UpdateSetupList();
    //void UpdateRebuildList();
    //void UpdateUnloadList();
    //void UpdateVisibleList();
    //void UpdateRenderList();

private:
    glm::vec3 _cameraPosition;
    glm::vec3 _cameraView;
    std::vector<Chunk>_chunks;
};

