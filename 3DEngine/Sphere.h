#pragma once
#include "IHittable.h"
#include "RT_Material.h"
#include <glm/gtx/norm.hpp>
#include <memory>

class Sphere: public IHittable
{
public:
    Sphere(glm::vec3 cen, float r, std::shared_ptr<RT_Material> m);
    ~Sphere();

    virtual bool hit(const Ray& r, float tmin, float tmax, hit_record& rec) const;

public:
    glm::vec3 center;
    float radius;
    std::shared_ptr<RT_Material> mat_ptr;
};

