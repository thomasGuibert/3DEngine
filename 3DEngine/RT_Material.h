#pragma once
#include "Ray.h"
#include "IHittable.h"

class RT_Material {
public:
    virtual bool scatter(
        const Ray& r_in, const hit_record& rec, glm::vec4& attenuation, Ray& scattered
    ) const = 0;
};