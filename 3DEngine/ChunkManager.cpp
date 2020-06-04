#include "ChunkManager.h"

ChunkManager::ChunkManager()
{

}

void ChunkManager::AddChunk(Shader& shader, Shader& shaderHighlight, glm::vec3 position) {
    Chunk chunk(shader, shaderHighlight, position);
    //chunk.Setup_Sphere();
    chunk.Setup_Landscape();
    //chunk.Setup_Cube();
    chunk.CreateMesh();
    _chunks.push_back(chunk);
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
        _chunks[ChunkIdX * SIZE + ChunkIdZ].setHighlightedBlock(BlockIdX, position.y, BlockIdZ);
        return true;
    }

    return false;
}

void ChunkManager::disableBlock(const glm::vec3 position)
{
    glm::vec4 block = getBlock(position);
    _chunks[block.x * SIZE + block.y].setHasChanged(true);

    return _chunks[block.x * SIZE + block.y].getBlock(block.z, position.y, block.w)->SetActive(false);
}

void ChunkManager::enableBlock(const glm::vec3 position)
{
    glm::vec4 block = getBlock(position);
    _chunks[block.x * SIZE + block.y].setHasChanged(true);
    return _chunks[block.x * SIZE + block.y].getBlock(block.z, position.y, block.w)->SetActive(true);
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
    return _chunks[block.x * SIZE + block.y].getBlock(block.z, position.y, block.w)->IsActive();
}

void ChunkManager::Update() {
    std::vector<Chunk>::iterator iterator;
    for (iterator = _chunks.begin(); iterator != _chunks.end(); ++iterator)
    {
        Chunk* pChunk = &(*iterator);
        pChunk->Render();
    }
}

ChunkManager::~ChunkManager()
{
}
