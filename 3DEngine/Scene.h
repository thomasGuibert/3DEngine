#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <string>
#include <exception>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <Camera.h>

class Scene
{
public:
    Scene(Camera &camera);
    virtual void render() = 0;
    ~Scene();

protected:
    Camera &_camera;
};

