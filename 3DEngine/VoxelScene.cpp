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
    camera.setChunkManager(chunkManager);
    skybox = new Skybox("../Assets/skybox/", _camera);
}

void VoxelScene::render()
{
    glm::mat4 view;
    glm::mat4 projection;

    skybox->render();
    projection = _camera.perspective();
    view = _camera.lookAt();
    _voxelShader.updateUniformMat4("projection", projection);
    _voxelShader.updateUniformMat4("view", view);
    _voxelshaderHighlight.updateUniformMat4("projection", projection);
    _voxelshaderHighlight.updateUniformMat4("view", view);

    int dist = 0;
    while (!chunkManager->setHighlightedBlock(_camera.getPosition() + _camera.getFront() * glm::vec3(dist)) && dist < 5) {
        dist++;
    }
    //_chunkManager->disableBlock(_position + _front * glm::vec3(dist));


    chunkManager->Update();

}


VoxelScene::~VoxelScene()
{
}
