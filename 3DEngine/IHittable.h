#pragma once
#include <glm/glm.hpp>
#include "Ray.h"

struct hit_record {
    glm::vec3 position;
    glm::vec3 normal;
    bool front_face;
    double t;

    inline void set_face_normal(const Ray& r, const glm::vec3& outward_normal) {
        front_face = dot(r.direction(), outward_normal) < 0;
        normal = front_face ? outward_normal : -outward_normal;
    }
};

class IHittable
{
public:
    virtual bool hit(const Ray& ray, float t_min, float t_max, hit_record& rec) const = 0;
};