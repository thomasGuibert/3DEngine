#pragma once
#include "Camera.h"

enum Action {
    ADD,
    REMOVE
};

enum Direction {
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT
};

class BaseCameraBehavior
{
public:
    BaseCameraBehavior(Camera& camera);
    ~BaseCameraBehavior();

    virtual void processKeyboardDirection(Direction direction, float deltaTime) = 0;
    virtual void processKeyboardAction(Action action) = 0;

    Camera& getCamera();
protected:
    Camera& _camera;
    float _speed = 2.0f;
};

