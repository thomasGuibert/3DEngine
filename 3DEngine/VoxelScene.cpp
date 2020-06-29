#include "VoxelScene.h"

VoxelScene::VoxelScene(VoxelCameraBehavior& manipulator) : Scene(manipulator),
_voxelShader("./shaders/voxelVertexShader.vs", "./shaders/voxelFragmentShader.fs"),
_voxelShaderHighlight("./shaders/voxelVertexShaderHighlight.vs", "./shaders/voxelFragmentShaderHighlight.fs"),
_viewfinderShader("./shaders/viewfinderVertexShader.vs", "./shaders/viewfinderFragmentShader.fs", "./shaders/viewfinderGeometryShader.gs"),
_texture()
{
    _texture = new ImageTexture("../Assets/terrain.png", "texture_diffuse");

    _chunkManager = new ChunkManager();
    glm::vec3 pos = glm::vec3(0, 0, 0);
    glm::vec3 cameraPosition = _manipulator.getCamera().getPosition();
    for (float x =0; x < _chunkManager->SIZE; ++x) {
        for (float z = 0; z < _chunkManager->SIZE; ++z) {
            pos = glm::vec3(x * 16, 0, z * 16);
            _chunkManager->AddChunk(_voxelShader, _voxelShaderHighlight, pos);
        }
    }

    manipulator.getCamera().setPosition(glm::vec3(11, 6, 8));
    manipulator.setChunkManager(_chunkManager);
    skybox = new Skybox("../Assets/skybox/", manipulator.getCamera());

    createCursor();
}

void VoxelScene::createCursor()
{
    unsigned int VBO;
    glGenVertexArrays(1, &_cursorVAO);
    glGenBuffers(1, &VBO);

    float screenCenter[] = { 0.0f, 0.0f, 0.0f };
    glBindVertexArray(_cursorVAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, 3 * sizeof(float), screenCenter, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
}

void VoxelScene::render()
{
    glm::mat4 view;
    glm::mat4 projection;

    if (_manipulator.getCamera().hasMoved()) {
        glm::vec3 pos = glm::vec3(0, 0, 0);
        glm::vec3 cameraBlockPosition = _chunkManager->getBlock(_manipulator.getCamera().getPosition());
        for (float x = cameraBlockPosition.x - _chunkManager->SIZE; x < cameraBlockPosition.x + _chunkManager->SIZE; ++x) {
            for (float z = cameraBlockPosition.y - _chunkManager->SIZE; z < cameraBlockPosition.y + _chunkManager->SIZE; ++z) {
                pos = glm::vec3(x * 16, 0, z * 16);
                _chunkManager->AddChunk(_voxelShader, _voxelShaderHighlight, pos);
            }
        }
    }
    skybox->render();
    projection = _manipulator.getCamera().perspective();
    view = _manipulator.getCamera().lookAt();
    _voxelShader.updateUniformMat4("projection", projection);
    _voxelShader.updateUniformMat4("view", view);
    _voxelShaderHighlight.updateUniformMat4("projection", projection);
    _voxelShaderHighlight.updateUniformMat4("view", view);

    int dist = 0;
    while (!_chunkManager->setHighlightedBlock(_manipulator.getCamera().getPosition() + _manipulator.getCamera().getFront() * glm::vec3(dist)) && dist < 5) {
        dist++;
    }
    _texture->Enable(0);
    _chunkManager->Update();

    glm::vec3 position = _manipulator.getCamera().getPosition() + _manipulator.getCamera().getFront();
    glm::mat4 model(1.0f);
    model = glm::translate(model, position);

    _viewfinderShader.updateUniformMat4("projection", projection);
    _viewfinderShader.updateUniformMat4("model", model);
    _viewfinderShader.updateUniformMat4("view", view);

    glUseProgram(_viewfinderShader.shaderProgramId);
    glBindVertexArray(_cursorVAO);
    glDrawArrays(GL_POINTS, 0, 3);
}


VoxelScene::~VoxelScene()
{
}
