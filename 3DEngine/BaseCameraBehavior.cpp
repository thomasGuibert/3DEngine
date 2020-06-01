#include "BaseCameraBehavior.h"

BaseCameraBehavior::BaseCameraBehavior(Camera& camera) : _camera(camera)
{
}

BaseCameraBehavior::~BaseCameraBehavior()
{
}

Camera& BaseCameraBehavior::getCamera()
{
    return _camera;
}
