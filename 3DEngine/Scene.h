#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <string>
#include <exception>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <BaseCameraBehavior.h>

class Scene
{
public:
    Scene(BaseCameraBehavior &manipulator);
    virtual void render() = 0;
    ~Scene();

protected:
    BaseCameraBehavior& _manipulator;
};

