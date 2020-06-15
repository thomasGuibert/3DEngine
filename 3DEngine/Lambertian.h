#pragma once
#include "RT_Material.h"
class Lambertian :
    public RT_Material
{
public:
    Lambertian(const glm::vec4& a);
    ~Lambertian();

    virtual bool scatter(
        const Ray& r_in, const hit_record& rec, glm::vec4& attenuation, Ray& scattered
    ) const;

public:
    glm::vec4 albedo;
};
