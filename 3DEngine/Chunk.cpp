#include "Chunk.h"
#include <iostream>


static int SOUTH = 0;
static int NORTH = 1;
static int EAST = 2;
static int WEST = 3;
static int TOP = 4;
static int BOTTOM = 5;

Chunk::Chunk(Shader& shader, Shader& shaderHighlight, glm::vec3 position) : _shader(shader), _shaderHighlight(shaderHighlight), _position(position)
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

    _voxelsFace = new VoxelFace**[CHUNK_SIZE];
    for (int i = 0; i < CHUNK_SIZE; i++)
    {
        _voxelsFace[i] = new VoxelFace*[CHUNK_SIZE];

        for (int j = 0; j < CHUNK_SIZE; j++)
        {
            _voxelsFace[i][j] = new VoxelFace[CHUNK_SIZE];
        }
    }
    _blockRenderer = new GL_Block();
}

void Chunk::CreateMesh()
{
    VoxelFace* face;
    //blockRenderer = new GL_Block();
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

                //glm::vec3 offset(x, y, z);
                //CreateCube(*blockRenderer, x, y, z, offset);
                face = new VoxelFace();
                face->type = m_pBlocks[x][y][z].GetBlockType();
                _voxelsFace[x][y][z] = *face;

            }
        }
    }

    greedy();
    _blockRenderer->build();
}



void Chunk::greedy() {

    /*
     * These are just working variables for the algorithm - almost all taken
     * directly from Mikola Lysenko's javascript implementation.
     */
    int i, j, k, l, w, h, u, v, n, side = 0;

    int x[3] = { 0, 0, 0 };
    int q[3] = { 0, 0, 0 };
    int du[3] = { 0, 0, 0 };
    int dv[3] = { 0, 0, 0 };

    /*
     * We create a mask - this will contain the groups of matching voxel faces
     * as we proceed through the chunk in 6 directions - once for each face.
     */
    VoxelFace* mask[CHUNK_SIZE*CHUNK_SIZE];

    /*
     * These are just working variables to hold two faces during comparison.
     */
    VoxelFace* voxelFace;
    VoxelFace* voxelFace1;

    /**
     * We start with the lesser-spotted boolean for-loop (also known as the old flippy floppy).
     *
     * The variable backFace will be TRUE on the first iteration and FALSE on the second - this allows
     * us to track which direction the indices should run during creation of the quad.
     *
     * This loop runs twice, and the inner loop 3 times - totally 6 iterations - one for each
     * voxel face.
     */
    for (bool backFace = true, b = false; b != backFace; backFace = backFace && b, b = !b) {

        /*
         * We sweep over the 3 dimensions - most of what follows is well described by Mikola Lysenko
         * in his post - and is ported from his Javascript implementation.  Where this implementation
         * diverges, I've added commentary.
         */
        for (int d = 0; d < 3; d++) {

            u = (d + 1) % 3;
            v = (d + 2) % 3;

            x[0] = 0;
            x[1] = 0;
            x[2] = 0;

            q[0] = 0;
            q[1] = 0;
            q[2] = 0;
            q[d] = 1;

            /*
             * Here we're keeping track of the side that we're meshing.
             */
            if (d == 0) { side = backFace ? WEST : EAST; }
            else if (d == 1) { side = backFace ? BOTTOM : TOP; }
            else if (d == 2) { side = backFace ? SOUTH : NORTH; }

            /*
             * We move through the dimension from front to back
             */
            for (x[d] = -1; x[d] < CHUNK_SIZE;) {

                /*
                 * -------------------------------------------------------------------
                 *   We compute the mask
                 * -------------------------------------------------------------------
                 */
                n = 0;

                for (x[v] = 0; x[v] < CHUNK_SIZE; x[v]++) {

                    for (x[u] = 0; x[u] < CHUNK_SIZE; x[u]++) {

                        /*
                         * Here we retrieve two voxel faces for comparison.
                         */
                        voxelFace = (x[d] >= 0) ? getVoxelFace(x[0], x[1], x[2], side) : NULL;
                        voxelFace1 = (x[d] < CHUNK_SIZE - 1) ? getVoxelFace(x[0] + q[0], x[1] + q[1], x[2] + q[2], side) : NULL;

                        /*
                         * Note that we're using the
                         equals function in the voxel face class here, which lets the faces
                         * be compared based on any number of attributes.
                         *
                         * Also, we choose the face to add to the mask depending on whether we're moving through on a backface or not.
                         */
                        mask[n++] = ((voxelFace != NULL && voxelFace1 != NULL && voxelFace->equals(voxelFace1)))
                            ? NULL
                            : backFace ? voxelFace1 : voxelFace;
                    }
                }

                x[d]++;

                /*
                 * Now we generate the mesh for the mask
                 */
                n = 0;

                for (j = 0; j < CHUNK_SIZE; j++) {

                    for (i = 0; i < CHUNK_SIZE;) {

                        if (mask[n] != NULL) {

                            /*
                             * We compute the width
                             */
                            for (w = 1; i + w < CHUNK_SIZE && mask[n + w] != NULL && mask[n + w]->equals(mask[n]); w++) {}

                            /*
                             * Then we compute height
                             */
                            bool done = false;

                            for (h = 1; j + h < CHUNK_SIZE; h++) {

                                for (k = 0; k < w; k++) {

                                    if (mask[n + k + h * CHUNK_SIZE] == NULL || !mask[n + k + h * CHUNK_SIZE]->equals(mask[n])) { done = true; break; }
                                }

                                if (done) { break; }
                            }

                            /*
                             * Here we check the "transparent" attribute in the VoxelFace class to ensure that we don't mesh
                             * any culled faces.
                             */
                            if (!mask[n]->transparent) {
                                /*
                                 * Add quad
                                 */
                                x[u] = i;
                                x[v] = j;

                                du[0] = 0;
                                du[1] = 0;
                                du[2] = 0;
                                du[u] = w;

                                dv[0] = 0;
                                dv[1] = 0;
                                dv[2] = 0;
                                dv[v] = h;

                                /*
                                 * And here we call the quad function in order to render a merged quad in the scene.
                                 *
                                 * We pass mask[n] to the function, which is an instance of the VoxelFace class containing
                                 * all the attributes of the face - which allows for variables to be passed to shaders - for
                                 * example lighting values used to create ambient occlusion.
                                 */
                                glm::vec3 bottomLeft(x[0], x[1], x[2]);
                                glm::vec3 topLeft(x[0] + du[0], x[1] + du[1], x[2] + du[2]);
                                glm::vec3 topRight(x[0] + du[0] + dv[0], x[1] + du[1] + dv[1], x[2] + du[2] + dv[2]);
                                glm::vec3 bottomRight(x[0] + dv[0], x[1] + dv[1], x[2] + dv[2]);
                                quad(bottomLeft,
                                    topLeft,
                                    topRight,
                                    bottomRight,
                                    w,
                                    h,
                                    mask[n],
                                    backFace);
                            }

                            /*
                             * We zero out the mask
                             */
                            for (l = 0; l < h; ++l) {

                                for (k = 0; k < w; ++k) { mask[n + k + l * CHUNK_SIZE] = NULL; }
                            }

                            /*
                             * And then finally increment the counters and continue
                             */
                            i += w;
                            n += w;

                        }
                        else {

                            i++;
                            n++;
                        }
                    }
                }
            }
        }
    }
}

void Chunk::quad(glm::vec3 bottomLeft,
    glm::vec3 topLeft,
    glm::vec3 topRight,
    glm::vec3 bottomRight,
    int width,
    int height,
    VoxelFace* voxel,
    bool backFace) {

    float vertexBuffer[18] = {
    bottomRight.x, bottomRight.y, bottomRight.z,
    bottomLeft.x, bottomLeft.y, bottomLeft.z,
    topLeft.x, topLeft.y, topLeft.z,
    topLeft.x, topLeft.y, topLeft.z,
    topRight.x, topRight.y, topRight.z,
    bottomRight.x, bottomRight.y, bottomRight.z };

    glm::vec3 directionRight = bottomRight - bottomLeft;
    glm::vec3 directionTop = topRight - bottomRight;
    glm::vec3 normal;

    if (backFace)
        normal = glm::cross(directionRight, directionTop);
    else
        normal = glm::cross(directionTop, directionRight);

    float normalBuffer[18] = {
        normal.x,normal.y,normal.z,
        normal.x,normal.y,normal.z,
        normal.x,normal.y,normal.z,
        normal.x,normal.y,normal.z,
        normal.x,normal.y,normal.z,
        normal.x,normal.y,normal.z
    };

    _blockRenderer->addFace(vertexBuffer, normalBuffer, voxel->type);
}

Block* Chunk::getBlock(const unsigned int x, const unsigned int y, const unsigned int z)
{
    //if(x > -1 && x < CHUNK_SIZE && z > -1 && z < CHUNK_SIZE && z > -1 && z < CHUNK_SIZE)
    return &m_pBlocks[x][y][z];
}

/**
 * This function returns an instance of VoxelFace containing the attributes for
 * one side of a voxel.  In this simple demo we just return a value from the
 * sample data array.  However, in an actual voxel engine, this function would
 * check if the voxel face should be culled, and set per-face and per-vertex
 * values as well as voxel values in the returned instance.
 *
 * @param x
 * @param y
 * @param z
 * @param face
 * @return
 */
VoxelFace* Chunk::getVoxelFace(int x, int y, int z, int side) {
    _voxelsFace[x][y][z].side = side;

    return &_voxelsFace[x][y][z];
}

void Chunk::CreateCube(GL_Block& blockRenderer, int x, int y, int z, glm::vec3 offset)
{
    //  if (x < CHUNK_SIZE - 1 && !m_pBlocks[x + 1][y][z].IsActive() || x == CHUNK_SIZE - 1)
    //      //_voxelsFace[x][y][z] = new VoxelFace();
    //      blockRenderer.addFace(FACE.RIGHT_FACE, NORMALS.RIGHT_FACE, offset);
    //
    //  if (x > 0 && !m_pBlocks[x - 1][y][z].IsActive() || x == 0)
    //      blockRenderer.addFace(FACE.LEFT_FACE, NORMALS.LEFT_FACE, offset);
    //
    //  if (z > 0 && !m_pBlocks[x][y][z - 1].IsActive() || z == 0)
    //      blockRenderer.addFace(FACE.BACK_FACE, NORMALS.BACK_FACE, offset);
    //
    //  if (z < CHUNK_SIZE - 1 && !m_pBlocks[x][y][z + 1].IsActive() || z == CHUNK_SIZE - 1)
    //      blockRenderer.addFace(FACE.FRONT_FACE, NORMALS.FRONT_FACE, offset);
    //
    //  if (y > 0 && !m_pBlocks[x][y - 1][z].IsActive() || y == 0)
    //      blockRenderer.addFace(FACE.BOTTOM_FACE, NORMALS.BOTTOM_FACE, offset);
    //
    //  if (y < CHUNK_SIZE - 1 && !m_pBlocks[x][y + 1][z].IsActive() || y == CHUNK_SIZE - 1)
    //      blockRenderer.addFace(FACE.TOP_FACE, NORMALS.TOP_FACE, offset);



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
                m_pBlocks[x][y][z].SetActive(true);
                if (y < 1) {
                    m_pBlocks[x][y][z].SetBlockType(BlockType_Water);
                }
                else {
                    if (y < 2) {
                        m_pBlocks[x][y][z].SetBlockType(BlockType_Ston);
                    }
                    else {
                        m_pBlocks[x][y][z].SetBlockType(BlockType_Grass);
                    }
                }
            }
        }
    }
}

void Chunk::Render()
{
    _blockRenderer->render(_shader, _shaderHighlight, _position);

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

void Chunk::setHighlightedBlock(int x, int y, int z)
{
    std::cout << "Block " << x << " " << y << " " << z << std::endl;

    float vertexBuffer[288] = {
        //FRONT
        0 + x, 0 + y, 0 + z,
        1 + x, 0 + y, 0 + z,
        1 + x, 1 + y, 0 + z,
        1 + x, 1 + y, 0 + z,
        0 + x, 1 + y, 0 + z,
        0 + x, 0 + y, 0 + z,

        //BACK
        0 + x, 0 + y, 1 + z,
        1 + x, 0 + y, 1 + z,
        1 + x, 1 + y, 1 + z,
        1 + x, 1 + y, 1 + z,
        0 + x, 1 + y, 1 + z,
        0 + x, 0 + y, 1 + z,

        //
        0 + x, 1 + y, 1 + z,
        0 + x, 1 + y, 0 + z,
        0 + x, 0 + y, 0 + z,
        0 + x, 0 + y, 0 + z,
        0 + x, 0 + y, 1 + z,
        0 + x, 1 + y, 1 + z,

        1 + x, 1 + y, 1 + z,
        1 + x, 1 + y, 0 + z,
        1 + x, 0 + y, 0 + z,
        1 + x, 0 + y, 0 + z,
        1 + x, 0 + y, 1 + z,
        1 + x, 1 + y, 1 + z,

        0 + x, 0 + y, 0 + z,
        1 + x, 0 + y, 0 + z,
        1 + x, 0 + y, 1 + z,
        1 + x, 0 + y, 1 + z,
        0 + x, 0 + y, 1 + z,
        0 + x, 0 + y, 0 + z,

        0 + x, 1 + y, 0 + z,
        1 + x, 1 + y, 0 + z,
        1 + x, 1 + y, 1 + z,
        1 + x, 1 + y, 1 + z,
        0 + x, 1 + y, 1 + z,
        0 + x, 1 + y, 0 + z
    };

    _blockRenderer->addHighlight(vertexBuffer);
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




