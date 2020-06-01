#pragma once
#include "Scene.h"

class EmptyScene : public Scene
{
public:
    EmptyScene(BaseCameraBehavior& manipulator);
    void render();
    ~EmptyScene();
};

