#include "CameraFPS.h"



CameraFPS::CameraFPS()
{
}

void CameraFPS::setChunkManager(ChunkManager & chunckManager)
{
    _chunckManager = chunckManager;

    while (_chunckManager.isActivatedBlock(_position))
        _position = glm::vec3(_position.x, _position.y + 1, _position.z);
    _position = glm::vec3(_position.x, _position.y, _position.z);
    std::cout << _position.x << " " << _position.y << " " << _position.z << " " << std::endl;
}

void CameraFPS::processKeyboard(Direction direction, float deltaTime)
{
    float velocity = _speed * deltaTime;

    if (ENABLE_HIHGLIGHT == true) {
        _chunckManager.setHighlightedBlock(_position + _front * glm::vec3(5));
    }
    //if (direction == FORWARD && !_chunckManager.isActivatedBlock(_position /*- glm::vec3(1.5,2.0,1.5)*/ + _front * velocity))
    //    _position += _front * velocity;
    //else {
    //    glm::vec3 p = _position + _front * velocity;
    //
    //    std::cout << "blocked!! " << p.x << " " << p.y << " " << p.z << std::endl;
    //}

    if (direction == FORWARD)
        _position += _front ;
    if (direction == BACKWARD)
        _position -= _front;
    if (direction == LEFT)
        _position -= glm::normalize(glm::cross(_front, _up));
    if (direction == RIGHT)
        _position += glm::normalize(glm::cross(_front, _up));

    std::cout << "camera: " << _position.x << " " << _position.y << " " << _position.z << std::endl;
}


CameraFPS::~CameraFPS()
{
}
