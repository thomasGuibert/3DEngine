#include "DefaultCameraBehavior.h"

DefaultCameraBehavior::DefaultCameraBehavior(Camera& camera) : BaseCameraBehavior(camera)
{
}

DefaultCameraBehavior::~DefaultCameraBehavior()
{
}

void DefaultCameraBehavior::processKeyboardDirection(Direction direction, float deltaTime)
{
    float velocity = _speed * deltaTime;
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

    _camera.setPosition(nextPosition);
    std::cout << "camera: " << _camera.getPosition().x << " " << _camera.getPosition().y << " " << _camera.getPosition().z << std::endl;
}

void DefaultCameraBehavior::processKeyboardAction(Action action)
{
}
