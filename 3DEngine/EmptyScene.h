#pragma once
#include "Scene.h"
class EmptyScene : public Scene
{
public:
    EmptyScene(Camera &camera);
    void render();
    ~EmptyScene();
};

