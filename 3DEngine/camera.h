#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

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
    Camera();
    Camera(Camera& camera);
    ~Camera();

    glm::mat4 lookAt() {
        return glm::lookAt(_position, _position + _front, _up);
    };
    glm::mat4 perspective() {
        return glm::perspective(glm::radians(_fov), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
    };

    void ProcessKeyboard(Direction direction, float deltaTime)
    {
        float velocity = _speed * deltaTime;
        if (direction == FORWARD)
            _position += _front * velocity;
        if (direction == BACKWARD)
            _position -= _front * velocity;
        if (direction == LEFT)
            _position -= glm::normalize(glm::cross(_front, _up)) * velocity;
        if (direction == RIGHT)
            _position += glm::normalize(glm::cross(_front, _up)) * velocity;
    };

    void scrollUpdate(double yoffset) {
        if (_fov >= 1.0f && _fov <= 45.0f)
            _fov -= yoffset;
        else if (_fov < 1.0f)
            _fov = 1.0f;
        else if (_fov > 45.0f)
            _fov = 45.0f;
    };

    void mouseUpdate(double xpos, double ypos) {
        if (firstMouse) // initially set to true
        {
            lastMousePosition.x = xpos;
            lastMousePosition.y = ypos;
            firstMouse = false;
        }

        double xOffset = lastMousePosition.x - xpos;
        double yOffset = lastMousePosition.y - ypos;
        lastMousePosition.x = xpos;
        lastMousePosition.y = ypos;

        const float sensitivity = 0.05f;
        xOffset *= sensitivity;
        yOffset *= sensitivity;

        _yaw += xOffset;
        _pitch -= yOffset;

        if (_pitch > 89.0f)
            _pitch = 89.0f;
        if (_pitch < -89.0f)
            _pitch = -89.0f;

        glm::vec3 direction;
        direction.x = glm::cos(glm::radians(_yaw)) * glm::cos(glm::radians(_pitch));
        direction.y = glm::sin(glm::radians(_pitch));
        direction.z = glm::sin(glm::radians(_yaw)) * glm::cos(glm::radians(_pitch));
        _front = glm::normalize(direction);
    }
    bool firstMouse = true;

private:
    glm::vec3 _position = glm::vec3(0.0f, 0.0f, 3.0f);
    glm::vec3 _front = glm::vec3(0.0f, 0.0f, -1.0f);
    glm::vec3 _up = glm::vec3(0.0f, 1.0f, 0.0f);
    float _yaw = -90.0f;
    float _pitch = 0.0f;
    float _fov = 45.0f;
    float _speed = 2.0f;
    MousePosition lastMousePosition;
};

