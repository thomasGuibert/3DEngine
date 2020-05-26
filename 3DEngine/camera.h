#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

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

struct MousePosition {
    double x = 400.0;
    double y = 300.0;
    double z = 0.0;
};



class Camera
{
public:
    bool isMousePressed = true;

    Camera();
    ~Camera();
    glm::mat4 lookAt();
    glm::mat4 perspective();
    glm::vec3 getPosition();
    glm::vec3 getDirection();
    glm::vec3 getFront();
    virtual void processKeyboardDirection(Direction direction, float deltaTime);
    virtual void processKeyboardAction(Action action) = 0;
    virtual void scrollUpdate(double yoffset);
    virtual void mouseUpdate(double xpos, double ypos);

protected:
    glm::vec3 _position = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::vec3 _front = glm::vec3(0.0f, 0.0f, -1.0f);
    glm::vec3 _up = glm::vec3(0.0f, 1.0f, 0.0f);
    float _speed = 2.0f;

private:

    float _yaw = -90.0f;
    float _pitch = 0.0f;
    float _fov = 45.0f;
    
    MousePosition _lastMousePosition;
};

