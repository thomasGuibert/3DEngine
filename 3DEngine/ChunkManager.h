#pragma once
#include "Chunk.h"
#include <unordered_map>

struct VectorHasher
{
    std::size_t operator()(const glm::vec3& vector) const
    {
        return ((std::hash<float>()(vector.x)
            ^ (std::hash<float>()(vector.y) << 1)) >> 1)
            ^ (std::hash<float>()(vector.z) << 1);
    }
};

class ChunkManager
{
public:
    ChunkManager();
    ~ChunkManager();
    static const unsigned int SIZE = 2;
 
    void Update();
    void AddChunk(Shader& shader, Shader& shaderHighlight, glm::vec3 position);
    bool setHighlightedBlock(const glm::vec3 position);
    void disableBlock(const glm::vec3 position);
    void enableBlock(const glm::vec3 position);
    glm::vec4 getBlock(const glm::vec3 position);
    bool isActivatedBlock(const glm::vec3 position);
private:
    std::unordered_map<glm::vec3, Chunk, VectorHasher> _chunks;
};

