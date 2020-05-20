#include "Chunk.h"
#include <iostream>

Chunk::Chunk(Shader& shader, glm::vec3 position) : _shader(shader), _position(position)
{
    // Create the blocks
    m_pBlocks = new Block**[CHUNK_SIZE];
    for (int i = 0; i < CHUNK_SIZE; i++)
    {
        m_pBlocks[i] = new Block*[CHUNK_SIZE];

        for (int j = 0; j < CHUNK_SIZE; j++)
        {
            m_pBlocks[i][j] = new Block[CHUNK_SIZE];
        }
    }
}

void Chunk::CreateMesh()
{
    blockRenderer = new GL_Block();
    for (int x = 0; x < CHUNK_SIZE; x++)
    {
        for (int y = 0; y < CHUNK_SIZE; y++)
        {
            for (int z = 0; z < CHUNK_SIZE; z++)
            {
                if (m_pBlocks[x][y][z].IsActive() == false)
                {
                    // Don't create triangle data for inactive blocks
                    continue;
                }

                glm::vec3 offset(x, y, z);
                CreateCube(*blockRenderer, x, y, z, offset);
            }
        }
    }

    blockRenderer->build();
}

void Chunk::CreateCube(GL_Block& blockRenderer, int x, int y, int z, glm::vec3 offset)
{


    if (x < CHUNK_SIZE - 1 && !m_pBlocks[x + 1][y][z].IsActive() || x == CHUNK_SIZE - 1)
        blockRenderer.addFace(FACE.RIGHT_FACE, NORMALS.RIGHT_FACE, offset);

    if (x > 0 && !m_pBlocks[x - 1][y][z].IsActive() || x == 0)
        blockRenderer.addFace(FACE.LEFT_FACE, NORMALS.LEFT_FACE, offset);


    if (z > 0 && !m_pBlocks[x][y][z - 1].IsActive() || z == 0)
        blockRenderer.addFace(FACE.BACK_FACE, NORMALS.BACK_FACE, offset);

    if (z < CHUNK_SIZE - 1 && !m_pBlocks[x][y][z + 1].IsActive() || z == CHUNK_SIZE - 1)
        blockRenderer.addFace(FACE.FRONT_FACE, NORMALS.FRONT_FACE, offset);

    if (y > 0 && !m_pBlocks[x][y - 1][z].IsActive() || y == 0)
        blockRenderer.addFace(FACE.BOTTOM_FACE, NORMALS.BOTTOM_FACE, offset);

    if (y < CHUNK_SIZE - 1 && !m_pBlocks[x][y + 1][z].IsActive() || y == CHUNK_SIZE - 1)
        blockRenderer.addFace(FACE.TOP_FACE, NORMALS.TOP_FACE, offset);



}

void Chunk::Setup_Sphere()
{
    for (int z = 0; z < CHUNK_SIZE; z++)
    {
        for (int y = 0; y < CHUNK_SIZE; y++)
        {
            for (int x = 0; x < CHUNK_SIZE; x++)
            {
                if (sqrt((float)(x - CHUNK_SIZE / 2)*(x - CHUNK_SIZE / 2) + (y - CHUNK_SIZE / 2)*(y - CHUNK_SIZE / 2) + (z - CHUNK_SIZE / 2)*(z - CHUNK_SIZE / 2)) <= CHUNK_SIZE / 2)
                {
                    m_pBlocks[x][y][z].SetActive(true);

                    // m_pBlocks[x][y][z].SetBlockType(BlockType_Grass);
                }
            }
        }
    }
}

void Chunk::Setup_Landscape()
{
    const unsigned int size = 50;
    std::vector<std::vector<float>> heightsMap;
    for (float x = 0.0f; x < CHUNK_SIZE; x += 1.0f) {
        std::vector<float> heights;
        for (float z = 0.0f; z < CHUNK_SIZE; z += 1.0f) {
            float height = ((glm::simplex(glm::vec2{ (x + _position.x) / 32, (z + _position.z) / 32 }) + 1) / 2) * 5;
            height *= ((glm::simplex(glm::vec2{ (x + _position.x) / 16, (z + _position.z) / 16 }) + 1) / 2) * 2;
            height += ((glm::simplex(glm::vec2{ (x + _position.x) / 8, (z + _position.z) / 8 }) + 1) / 2) * 2;
            height /= CHUNK_SIZE;
            heights.push_back(height);
        }
        heightsMap.push_back(heights);
    }

    for (int x = 0; x < CHUNK_SIZE; x++)
    {
        for (int z = 0; z < CHUNK_SIZE; z++)
        {
            // Use the noise library to get the height value of x, z
            //float height = m_pChunkManager->GetNoiseValue(x, z);

            // Use the height map texture to get the height value of x, z
            float height = (heightsMap[x][z] * (CHUNK_SIZE - 1) * 1.0f) * 1.0f;

            for (int y = 0; y < height; y++)
            {
                std::cout << x << " " << y << " " << z << std::endl;
                m_pBlocks[x][y][z].SetActive(true);
                //m_pBlocks[x][y][z].SetBlockType(BlockType_Grass);
            }
        }
    }
}

void Chunk::Render()
{
    blockRenderer->render(_shader, _position);

    //pRenderer->PushMatrix();
    //pRenderer->ImmediateColourAlpha(1.0f, 1.0f, 1.0f, 1.0f);
    //pRenderer->SetRenderMode(RM_TEXTURED_LIGHTING);
    //
    //float x = m_position.x;
    //float y = m_position.y;
    //float z = m_position.z;
    //pRenderer->TranslateWorldMatrix(x, y, z);
    //
    //if (m_meshID != -1)
    //{
    //    pRenderer->RenderMesh(m_meshID);
    //}
    //pRenderer->PopMatrix();
}

Chunk::~Chunk()
{
    // Delete the blocks
   //for (int i = 0; i < CHUNK_SIZE; ++i)
   //{
   //    for (int j = 0; j < CHUNK_SIZE; ++j)
   //    {
   //        delete[] m_pBlocks[i][j];
   //    }
   //
   //    delete[] m_pBlocks[i];
   //}
   //delete[] m_pBlocks;
}




