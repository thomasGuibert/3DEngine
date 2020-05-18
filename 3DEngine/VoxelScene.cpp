#include "VoxelScene.h"
#include <glm/ext.hpp>

VoxelScene::VoxelScene(Camera & camera) : Scene(camera),
_voxelShader("./shaders/voxelVertexShader.vs", "./shaders/voxelFragmentShader.fs"),
_texture()
{
    _voxel = new Model(_vertices, sizeof(_vertices), 2, _voxelShader);
    _texture = new ImageTexture("../Assets/containerWithIron.png", "texture_diffuse");
    //_texture = new ImageTexture("../Assets/minecraft/textures/block/green_concrete_powder.png", "texture_diffuse");

    _texture->Enable(0);
    _voxelShader.updateUniformInt("voxelTexture", 0);
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

    for (float x = 0.0f; x < 10; x += 0.4f) {
        for (float z = 0.0f; z < 10; z += 0.4f) {
            float height = ((glm::simplex(glm::vec2{ x / 16, z / 16 }) + 1) / 2) * 5;
            for (float y = 0.0f; y <= height; y += 0.4f) {
                _voxel->drawOnPosition(x, y, z);
            }
        }
    }
}


VoxelScene::~VoxelScene()
{
}
