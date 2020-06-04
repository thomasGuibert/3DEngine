#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

struct MousePosition {
    double x = 400.0;
    double y = 300.0;
    double z = 0.0;
};

class Camera
{
public:
    Camera();
    ~Camera();
    glm::mat4 lookAt();
    glm::mat4 perspective();
    glm::vec3 getPosition();
    glm::vec3 getDirection();
    glm::vec3 getFront();
    void setPosition(glm::vec3 position);

    glm::vec3 getMoveFrontPosition(float velocity);
    glm::vec3 getMoveBackPosition(float velocity);
    glm::vec3 getMoveLeftPosition(float velocity);
    glm::vec3 getMoveRightPosition(float velocity);

    virtual void scrollUpdate(double yoffset);
    virtual void mouseUpdate(double xpos, double ypos);

protected:
    glm::vec3 _position = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::vec3 _front = glm::vec3(0.0f, 0.0f, -1.0f);
    glm::vec3 _up = glm::vec3(0.0f, 1.0f, 0.0f);

private:
    bool _firstMouse = true;
    float _yaw = -90.0f;
    float _pitch = 0.0f;
    float _fov = 45.0f;

    MousePosition _lastMousePosition;
};

