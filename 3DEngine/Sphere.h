#pragma once
#include "IHittable.h"
#include <glm/gtx/norm.hpp>

class Sphere: public IHittable
{
public:
    Sphere(glm::vec3 cen, float r); 
    ~Sphere();

    virtual bool hit(const Ray& r, float tmin, float tmax, hit_record& rec) const;

public:
    glm::vec3 center;
    float radius;
};

