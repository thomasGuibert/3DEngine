#pragma once
#include <iostream>
#include "BaseCameraBehavior.h"

class DefaultCameraBehavior: public BaseCameraBehavior
{
public:
    DefaultCameraBehavior(Camera& camera);
    ~DefaultCameraBehavior();
    virtual void processKeyboardDirection(Direction direction, float deltaTime);
    virtual void processKeyboardAction(Action action);
};

