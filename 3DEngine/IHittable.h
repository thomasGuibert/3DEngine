#pragma once
#include <glm/glm.hpp>
#include <memory>
#include "Ray.h"

class RT_Material;
struct hit_record {
    glm::vec3 position;
    glm::vec3 normal;
    bool front_face;
    double t;
    std::shared_ptr<RT_Material> mat_ptr;
    
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