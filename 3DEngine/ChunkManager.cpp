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

    std::cout << "Chunk " << ChunkIdX << " " << ChunkIdZ << std::endl;

    if (isActivatedBlock(position)) {
        _chunks[ChunkIdX * SIZE + ChunkIdZ].setHighlightedBlock(BlockIdX, position.y, BlockIdZ);
        return true;
    }
    std::cout << "Activated " << isActivatedBlock(position) << std::endl;
    std::cout << std::endl;
    std::cout << std::endl;
    return false;
}

void ChunkManager::disableBlock(const glm::vec3 position)
{
    glm::vec4 block = getBlock(position);
    _chunks[block.x * SIZE + block.y].setHasChanged(true);
    return _chunks[block.x * SIZE + block.y].getBlock(block.z, position.y, block.w)->SetActive(false);

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
    for (iterator = _chunks.begin(); iterator != _chunks.end() /*&&
        (lNumOfChunksLoaded != ASYNC_NUM_CHUNKS_PER_FRAME)*/; ++iterator)
    {
        Chunk* pChunk = &(*iterator);
        pChunk->Render();
    }
}
//void ChunkManager::Update(float dt, glm::vec3 cameraPosition, glm::vec3 cameraView)
//{
//    UpdateLoadList();
//
//    UpdateSetupList();
//
//    UpdateRebuildList();
//
//    UpdateUnloadList();
//
//    //UpdateVisibilityList(cameraPosition);
//
//    if (_cameraPosition != cameraPosition || _cameraView != cameraView)
//    {
//        UpdateRenderList();
//    }
//
//    _cameraPosition = cameraPosition;
//    _cameraView = cameraView;
//}
//
//void ChunkManager::UpdateLoadList()
//{
//    int lNumOfChunksLoaded = 0;
//
//    std::vector<Chunk>::iterator iterator;
//    for (iterator = _chunks.begin(); iterator != _chunks.end() /*&&
//        (lNumOfChunksLoaded != ASYNC_NUM_CHUNKS_PER_FRAME)*/; ++iterator)
//    {
//        Chunk* pChunk = &(*iterator);
//
//        if (pChunk->IsLoaded() == false)
//        {
//            if (lNumOfChunksLoaded != ASYNC_NUM_CHUNKS_PER_FRAME))
//            {
//            pChunk->Load();
//
//            // Increase the chunks loaded count
//            lNumOfChunksLoaded++;
//
//            m_forceVisibilityUpdate = true;
//            }
//        }
//    }
//
//    // Clear the load list (every frame)
//    m_vpChunkLoadList.clear();
//}
//
//void ChunkManager::UpdateSetupList()
//{
//    // Setup any chunks that have not already been setup
//    ChunkList::iterator iterator;
//    for (iterator = m_vpChunkSetupList.begin(); iterator != m_vpChunkSetupList.end(); ++iterator)
//    {
//        Chunk* pChunk = (*iterator);
//
//        if (pChunk->IsLoaded() && pChunk->IsSetup() == false)
//        {
//            pChunk->Setup();
//
//            if (pChunk->IsSetup())
//            {
//                // Only force the visibility update if we actually setup the chunk, some chunks wait in the pre-setup stage...
//                m_forceVisibilityUpdate = true;
//            }
//        }
//    }
//
//    // Clear the setup list (every frame)
//    m_vpChunkSetupList.clear();
//}
//
//void ChunkManager::UpdateRebuildList()
//{
//    // Rebuild any chunks that are in the rebuild chunk list
//    ChunkList::iterator iterator;
//    int lNumRebuiltChunkThisFrame = 0;
//    for (iterator = m_vpChunkRebuildList.begin(); iterator != m_vpChunkRebuildList.end() &&
//        (lNumRebuiltChunkThisFrame != ASYNC_NUM_CHUNKS_PER_FRAME); ++iterator)
//    {
//        Chunk* pChunk = (*iterator);
//
//        if (pChunk->IsLoaded() && pChunk->IsSetup())
//        {
//            if (lNumRebuiltChunkThisFrame != ASYNC_NUM_CHUNKS_PER_FRAME)
//            {
//                pChunk->RebuildMesh();
//
//                // If we rebuild a chunk, add it to the list of chunks that need their render flags updated
//                // since we might now be empty or surrounded
//                m_vpChunkUpdateFlagsList.push_back(pChunk);
//
//                // Also add our neighbours since they might now be surrounded too (If we have neighbours)
//                Chunk* pChunkXMinus = GetChunk(pChunk->GetX() - 1, pChunk->GetY(), pChunk->GetZ());
//                Chunk* pChunkXPlus = GetChunk(pChunk->GetX() + 1, pChunk->GetY(), pChunk->GetZ());
//                Chunk* pChunkYMinus = GetChunk(pChunk->GetX(), pChunk->GetY() - 1, pChunk->GetZ());
//                Chunk* pChunkYPlus = GetChunk(pChunk->GetX(), pChunk->GetY() + 1, pChunk->GetZ());
//                Chunk* pChunkZMinus = GetChunk(pChunk->GetX(), pChunk->GetY(), pChunk->GetZ() - 1);
//                Chunk* pChunkZPlus = GetChunk(pChunk->GetX(), pChunk->GetY(), pChunk->GetZ() + 1);
//
//                if (pChunkXMinus != NULL)
//                    m_vpChunkUpdateFlagsList.push_back(pChunkXMinus);
//
//                if (pChunkXPlus != NULL)
//                    m_vpChunkUpdateFlagsList.push_back(pChunkXPlus);
//
//                if (pChunkYMinus != NULL)
//                    m_vpChunkUpdateFlagsList.push_back(pChunkYMinus);
//
//                if (pChunkYPlus != NULL)
//                    m_vpChunkUpdateFlagsList.push_back(pChunkYPlus);
//
//                if (pChunkZMinus != NULL)
//                    m_vpChunkUpdateFlagsList.push_back(pChunkZMinus);
//
//                if (pChunkZPlus != NULL)
//                    m_vpChunkUpdateFlagsList.push_back(pChunkZPlus);
//
//                // Only rebuild a certain number of chunks per frame
//                lNumRebuiltChunkThisFrame++;
//
//                m_forceVisibilityUpdate = true;
//            }
//        }
//    }
//
//    // Clear the rebuild list
//    m_vpChunkRebuildList.clear();
//}
//
//void ChunkManager::UpdateUnloadList()
//{
//    // Unload any chunks
//    ChunkList::iterator iterator;
//    for (iterator = m_vpChunkUnloadList.begin(); iterator != m_vpChunkUnloadList.end(); ++iterator)
//    {
//        Chunk* pChunk = (*iterator);
//
//        if (pChunk->IsLoaded())
//        {
//            pChunk->Unload();
//
//            m_forceVisibilityUpdate = true;
//        }
//    }
//
//    // Clear the unload list (every frame)
//    m_vpChunkUnloadList.clear();
//}
//
//void ChunkManager::UpdateVisibleList()
//{
//}
//
//void ChunkManager::UpdateRenderList()
//{
//    // Clear the render list each frame BEFORE we do our tests to see what chunks should be rendered
//    m_vpChunkRenderList.clear();
//
//    ChunkList::iterator iterator;
//    for (iterator = m_vpChunkVisibilityList.begin(); iterator != m_vpChunkVisibilityList.end(); ++iterator)
//    {
//        Chunk* pChunk = (*iterator);
//
//        if (pChunk != NULL)
//        {
//            if (pChunk->IsLoaded() && pChunk->IsSetup())
//            {
//                if (pChunk->ShouldRender()) // Early flags check so we don't always have to do the frustum check...
//                {
//                    // Check if this chunk is inside the camera frustum
//                    float c_offset = (Chunk::CHUNK_SIZE * Block::BLOCK_RENDER_SIZE) - Block::BLOCK_RENDER_SIZE;
//                    Vector3d chunkCenter = pChunk->GetPosition() + Vector3d(c_offset, c_offset, c_offset);
//
//                    float c_size = Chunk::CHUNK_SIZE * Block::BLOCK_RENDER_SIZE;
//                    if (m_pRenderer->CubeInFrustum(m_pRenderer->GetActiveViewPort(), chunkCenter, c_size, c_size, c_size))
//                    {
//                        m_vpChunkRenderList.push_back(pChunk);
//                    }
//                }
//            }
//        }
//    }
//}

ChunkManager::~ChunkManager()
{
}
