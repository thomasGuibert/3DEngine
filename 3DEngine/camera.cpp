#include "Camera.h"
#include <iostream>
Camera::Camera() {}

Camera::~Camera() {}

glm::mat4 Camera::lookAt() {
    return glm::lookAt(_position, _position + _front, _up);
};

glm::mat4 Camera::perspective() {
    return glm::perspective(glm::radians(_fov), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
};

glm::vec3 Camera::getPosition() {
    return _position;
}

glm::vec3 Camera::getDirection()
{
    return _front;
}

void Camera::processKeyboard(Direction direction, float deltaTime)
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

    std::cout << "camera: " << _position.x << _position.y << _position.z << std::endl;
};

void Camera::scrollUpdate(double yoffset) {
    if (_fov >= 1.0f && _fov <= 45.0f)
        _fov -= yoffset;
    else if (_fov < 1.0f)
        _fov = 1.0f;
    else if (_fov > 45.0f)
        _fov = 45.0f;
};

void Camera::mouseUpdate(double xpos, double ypos) {
    if (!isMousePressed)
    {
        _lastMousePosition.x = xpos;
        _lastMousePosition.y = ypos;
        isMousePressed = true;
    }

    double xOffset = xpos- _lastMousePosition.x;
    double yOffset = _lastMousePosition.y - ypos;
    _lastMousePosition.x = xpos;
    _lastMousePosition.y = ypos;

    const float sensitivity = 0.8f;
    xOffset *= sensitivity;
    yOffset *= sensitivity;

    _yaw += xOffset;
    _pitch += yOffset;

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