#pragma once
#include "Chunk.h"

class ChunkManager
{
public:
    ChunkManager();
    ~ChunkManager();
    static const unsigned int SIZE = 2;
 

    void Update();
    void AddChunk(Shader& shader, Shader& shaderHighlight, glm::vec3 position);
    //void Update(float dt, glm::vec3 cameraPosition, glm::vec3 cameraView);
    //void UpdateLoadList();
    //void UpdateSetupList();
    //void UpdateRebuildList();
    //void UpdateUnloadList();
    //void UpdateVisibleList();
    //void UpdateRenderList();
    bool setHighlightedBlock(const glm::vec3 position);
    void disableBlock(const glm::vec3 position);
    glm::vec4 getBlock(const glm::vec3 position);
    bool isActivatedBlock(const glm::vec3 position);
private:
    std::vector<Chunk>_chunks;
    //Camera& came
};

