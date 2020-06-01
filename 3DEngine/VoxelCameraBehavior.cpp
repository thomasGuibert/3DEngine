#include "VoxelCameraBehavior.h"

VoxelCameraBehavior::VoxelCameraBehavior(Camera & camera): BaseCameraBehavior(camera)
{
}

VoxelCameraBehavior::~VoxelCameraBehavior()
{
}


void VoxelCameraBehavior::setChunkManager(ChunkManager* chunckManager)
{
    _chunkManager = chunckManager;
    glm::vec3 cameraPosition = _camera.getPosition();
    while (cameraPosition.y < 16 && _chunkManager->isActivatedBlock(cameraPosition))
        cameraPosition = glm::vec3(cameraPosition.x, cameraPosition.y + 1, cameraPosition.z);
    cameraPosition = glm::vec3(cameraPosition.x, cameraPosition.y + 0.5, cameraPosition.z);
    _camera.setPosition(cameraPosition);
    std::cout << _camera.getPosition().x << " " << _camera.getPosition().y << " " << _camera.getPosition().z << " " << std::endl;
}

void VoxelCameraBehavior::processKeyboardDirection(Direction direction, float deltaTime)
{
    float velocity = _speed * deltaTime;
    glm::vec3 currentPosition = _camera.getPosition();
    glm::vec3 nextPosition;
    switch (direction)
    {
    case FORWARD:
        nextPosition = _camera.getMoveFrontPosition(velocity);
        break;
    case BACKWARD:
        nextPosition = _camera.getMoveBackPosition(velocity);
        break;
    case LEFT:
        nextPosition = _camera.getMoveLeftPosition(velocity);
        break;
    case RIGHT:
        nextPosition = _camera.getMoveRightPosition(velocity);
        break;
    default:
        throw;
        break;
    }

    if (!_chunkManager->isActivatedBlock(nextPosition))
        currentPosition = nextPosition;
    else
        std::cout << "blocked" << std::endl;

    currentPosition.y = 0;
    while (currentPosition.y < 16 && _chunkManager->isActivatedBlock(currentPosition))
        currentPosition = glm::vec3(currentPosition.x, currentPosition.y + 1, currentPosition.z);
    currentPosition = glm::vec3(currentPosition.x, currentPosition.y + 1, currentPosition.z);

    _camera.setPosition(currentPosition);
    std::cout << "camera: " << _camera.getPosition().x << " " << _camera.getPosition().y << " " << _camera.getPosition().z << std::endl;
}

void VoxelCameraBehavior::processKeyboardAction(Action action)
{
    if (action == Action::REMOVE) {
        int dist = 0;
        while (!_chunkManager->setHighlightedBlock(_camera.getMoveFrontPosition(1) * glm::vec3(dist)) && dist < 5) {
            dist++;
        }
        _chunkManager->disableBlock(_camera.getMoveFrontPosition(1) * glm::vec3(dist));
    }
    if (action == Action::ADD) {
        int dist = 0;
        while (!_chunkManager->setHighlightedBlock(_camera.getMoveFrontPosition(1) * glm::vec3(dist)) && dist < 5) {
            dist++;
        }
        glm::vec3 nextPosition(_camera.getPosition().x, _camera.getPosition().y + 1, _camera.getPosition().z);
        _chunkManager->enableBlock(nextPosition + _camera.getFront() * glm::vec3(dist));
    }
}
