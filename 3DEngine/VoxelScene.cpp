#include "VoxelScene.h"
#include <glm/ext.hpp>

VoxelScene::VoxelScene(CameraFPS & camera) : Scene(camera),
_voxelShader("./shaders/voxelVertexShader.vs", "./shaders/voxelFragmentShader.fs"),
_voxelshaderHighlight("./shaders/voxelVertexShaderHighlight.vs", "./shaders/voxelFragmentShaderHighlight.fs"),
_texture()
{
    //_texture = new ImageTexture("../Assets/containerWithIron.png", "texture_diffuse");
    //_texture = new ImageTexture("../Assets/minecraft/textures/block/green_concrete_powder.png", "texture_diffuse");
    _texture = new ImageTexture("../Assets/terrain.png", "texture_diffuse");

    _texture->Enable(0);
    _voxelShader.updateUniformInt("voxelTexture", 0);
    chunkManager = new ChunkManager();
    glm::vec3 pos = glm::vec3(0, 0, 0);
    for (float x = 0.0f; x < chunkManager->SIZE; ++x) {
        for (float z = 0.0f; z < chunkManager->SIZE; ++z) {
            pos = glm::vec3(x * 16, 0, z * 16);
            chunkManager->AddChunk(_voxelShader, _voxelshaderHighlight, pos);
        }
    }
    camera.setChunkManager(*chunkManager);

}

void VoxelScene::render()
{
    glm::mat4 view;
    glm::mat4 projection;

    projection = _camera.perspective();
    view = _camera.lookAt();
    _voxelShader.updateUniformMat4("projection", projection);
    _voxelShader.updateUniformMat4("view", view);
    _voxelshaderHighlight.updateUniformMat4("projection", projection);
    _voxelshaderHighlight.updateUniformMat4("view", view);

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
