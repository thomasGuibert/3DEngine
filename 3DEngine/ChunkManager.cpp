#include "ChunkManager.h"

ChunkManager::ChunkManager()
{

}

void ChunkManager::AddChunk(Shader& shader, Shader& shaderHighlight, glm::vec3 position) {
    if (_chunks.find(position) != _chunks.end())
        return;

    Chunk chunk(shader, shaderHighlight, position);
    //chunk.Setup_Sphere();
    chunk.Setup_Landscape();
    //chunk.Setup_Cube();
    chunk.CreateMesh();
    _chunks.insert(std::pair<glm::vec3, Chunk>(position, chunk));
}

bool ChunkManager::setHighlightedBlock(const glm::vec3 position)
{
    float chunkPostionX = (position.x / (SIZE * Chunk::CHUNK_SIZE))*SIZE;
    float chunkPostionZ = (position.z / (SIZE * Chunk::CHUNK_SIZE))*SIZE;

    int ChunkIdX = int(chunkPostionX);
    int ChunkIdZ = int(chunkPostionZ);

    int BlockIdX = (chunkPostionX - ChunkIdX) * Chunk::CHUNK_SIZE;
    int BlockIdZ = (chunkPostionZ - ChunkIdZ) * Chunk::CHUNK_SIZE;

    if (isActivatedBlock(position)) {
        glm::vec3 p(ChunkIdX, 0, ChunkIdZ);
        (_chunks.find(p)->second).setHighlightedBlock(BlockIdX, position.y, BlockIdZ);
        return true;
    }

    return false;
}

void ChunkManager::disableBlock(const glm::vec3 position)
{
    glm::vec4 block = getBlock(position);
    (_chunks.find(glm::vec3(block.x, 0, block.y))->second).setHasChanged(true);
    (_chunks.find(glm::vec3(block.x, 0, block.y))->second).getBlock(block.z, position.y, block.w)->SetActive(false);
}

void ChunkManager::enableBlock(const glm::vec3 position)
{
    glm::vec4 block = getBlock(position);
    (_chunks.find(glm::vec3(block.x, 0, block.y))->second).setHasChanged(true);
    (_chunks.find(glm::vec3(block.x, 0, block.y))->second).getBlock(block.z, position.y, block.w)->SetActive(true);
}

glm::vec4 ChunkManager::getBlock(const glm::vec3 position)
{
    float chunkPostionX = (position.x / (SIZE * Chunk::CHUNK_SIZE))*SIZE;
    float chunkPostionZ = (position.z / (SIZE * Chunk::CHUNK_SIZE))*SIZE;

    int ChunkIdX = int(chunkPostionX);
    int ChunkIdZ = int(chunkPostionZ);

    int BlockIdX = (chunkPostionX - ChunkIdX) * Chunk::CHUNK_SIZE;
    int BlockIdZ = (chunkPostionZ - ChunkIdZ) * Chunk::CHUNK_SIZE;

    return glm::vec4(ChunkIdX, ChunkIdZ, BlockIdX, BlockIdZ);
}

bool ChunkManager::isActivatedBlock(const glm::vec3 position)
{
    glm::vec4 block = getBlock(position);
    std::unordered_map<glm::vec3, Chunk>::iterator iterator = _chunks.find(glm::vec3(block.x, 0, block.y));
    if (iterator == _chunks.end())
        return false;
    Chunk chunk = iterator->second;
    Block* blockObj = chunk.getBlock(block.z, position.y, block.w);
    return blockObj->IsActive();
}

void ChunkManager::Update() {
    std::unordered_map<glm::vec3,Chunk>::iterator iterator = _chunks.begin();
    for (iterator = _chunks.begin(); iterator != _chunks.end(); iterator++)
    {
        (iterator->second).Render();
    }
}

ChunkManager::~ChunkManager()
{
}
