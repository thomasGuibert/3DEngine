#pragma once
#include <glm/glm.hpp>

class Ray
{
public:
    Ray();
    Ray(glm::vec3 origin, glm::vec3 direction);
    ~Ray();
    glm::vec3 origin() const { return orig; }
    glm::vec3 direction() const { return dir; }

    glm::vec3 at(float t) const {
        return orig + t * dir;
    }

    glm::vec3 orig;
    glm::vec3 dir;
};

