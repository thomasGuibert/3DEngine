#include "CameraFPS.h"



CameraFPS::CameraFPS()
{
}

void CameraFPS::setChunkManager(ChunkManager* chunckManager)
{
    _chunkManager = chunckManager;

    while (_position.y < 16 && _chunkManager->isActivatedBlock(_position))
        _position = glm::vec3(_position.x, _position.y + 1, _position.z);
    _position = glm::vec3(_position.x, _position.y + 0.5, _position.z);
    std::cout << _position.x << " " << _position.y << " " << _position.z << " " << std::endl;
}

void CameraFPS::processKeyboardDirection(Direction direction, float deltaTime)
{
    //_position = glm::vec3(_position.x, _position.y-1, _position.z);
    //_chunkManager->disableBlock(_position + _front);
    float velocity = _speed * deltaTime;
    glm::vec3 nextPosition;

    if (direction == FORWARD)
        nextPosition = _position + _front * velocity;
    if (direction == BACKWARD)
        nextPosition = _position - _front * velocity;
    if (direction == LEFT)
        nextPosition = _position - glm::normalize(glm::cross(_front, _up)) * velocity;
    if (direction == RIGHT)
        nextPosition = _position + glm::normalize(glm::cross(_front, _up)) * velocity;

    if (!_chunkManager->isActivatedBlock(nextPosition))
        _position = nextPosition;
    else
        std::cout << "blocked" << std::endl;

    _position.y = 0;
    while (_position.y < 16 && _chunkManager->isActivatedBlock(_position))
        _position = glm::vec3(_position.x, _position.y + 1, _position.z);
    _position = glm::vec3(_position.x, _position.y + 1, _position.z);
    std::cout << "camera: " << _position.x << " " << _position.y << " " << _position.z << std::endl;
}

void CameraFPS::processKeyboardAction(Action action)
{
    if (action == Action::REMOVE) {
        int dist = 0;
        while (!_chunkManager->setHighlightedBlock(_position + _front * glm::vec3(dist)) && dist < 5) {
            dist++;
        }
        _chunkManager->disableBlock(_position + _front * glm::vec3(dist));
    }
}


CameraFPS::~CameraFPS()
{
}
