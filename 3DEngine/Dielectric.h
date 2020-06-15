#pragma once
#include "RT_Material.h"
#include "Ray.h"
#include "RayTracingScene.h"
#include <glm/glm.hpp>

class Dielectric : public RT_Material
{
public:
    Dielectric(double ri);
    ~Dielectric();

    virtual bool scatter( const Ray& r_in, const hit_record& rec, glm::vec4& attenuation, Ray& scattered) const;

    double ref_idx;
};