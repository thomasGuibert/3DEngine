#include "VoxelScene.h"

VoxelScene::VoxelScene(VoxelCameraBehavior& manipulator) : Scene(manipulator),
_voxelShader("./shaders/voxelVertexShader.vs", "./shaders/voxelFragmentShader.fs"),
_voxelshaderHighlight("./shaders/voxelVertexShaderHighlight.vs", "./shaders/voxelFragmentShaderHighlight.fs"),
_texture()
{
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

    manipulator.getCamera().setPosition(glm::vec3(11, 6, 8));
    manipulator.setChunkManager(chunkManager);
    skybox = new Skybox("../Assets/skybox/", manipulator.getCamera());
}

void VoxelScene::render()
{
    glm::mat4 view;
    glm::mat4 projection;

    skybox->render();
    projection = _manipulator.getCamera().perspective();
    view = _manipulator.getCamera().lookAt();
    _voxelShader.updateUniformMat4("projection", projection);
    _voxelShader.updateUniformMat4("view", view);
    _voxelshaderHighlight.updateUniformMat4("projection", projection);
    _voxelshaderHighlight.updateUniformMat4("view", view);

    int dist = 0;
    while (!chunkManager->setHighlightedBlock(_manipulator.getCamera().getPosition() + _manipulator.getCamera().getFront() * glm::vec3(dist)) && dist < 5) {
        dist++;
    }

    chunkManager->Update();
}


VoxelScene::~VoxelScene()
{
}
