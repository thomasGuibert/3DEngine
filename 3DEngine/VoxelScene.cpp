#include "VoxelScene.h"
#include <glm/ext.hpp>

VoxelScene::VoxelScene(Camera & camera) : Scene(camera),
_voxelShader("./shaders/voxelVertexShader.vs", "./shaders/voxelFragmentShader.fs"),
_texture()
{
    //_texture = new ImageTexture("../Assets/containerWithIron.png", "texture_diffuse");
    _texture = new ImageTexture("../Assets/minecraft/textures/block/green_concrete_powder.png", "texture_diffuse");
    _texture->Enable(0);
    _voxelShader.updateUniformInt("voxelTexture", 0);
    chunkManager = new ChunkManager();
    glm::vec3 pos = glm::vec3(0, 0, 0);
    //for (float x = 0.0f; x < 2; ++x) {
    //    for (float z = 0.0f; z < 2; ++z) {
    //        pos = glm::vec3(x * 16, 0, z * 16);
            chunkManager->AddChunk(_voxelShader, pos);
    //    }
    //}
}

void VoxelScene::render()
{
    glm::mat4 view;
    glm::mat4 projection;

    projection = _camera.perspective();
    view = _camera.lookAt();
    glm::vec3 pos = _camera.getPosition();
    glm::vec3 direction = _camera.getDirection();
    _voxelShader.updateUniformMat4("projection", projection);
    _voxelShader.updateUniformMat4("view", view);

    chunkManager->Update();
    for (float x = 0.0f; x < 10; x += 0.4f) {
        for (float z = 0.0f; z < 10; z += 0.4f) {
            float height = ((glm::simplex(glm::vec2{ x / 16, z / 16 }) + 1) / 2) * 5;
            for (float y = 0.0f; y <= height; y += 0.4f) {

            }
        }
    }
}


VoxelScene::~VoxelScene()
{
}
