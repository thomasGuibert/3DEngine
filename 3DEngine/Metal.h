#pragma once
#include "RT_Material.h"
#include "RayTracingScene.h"

class Metal : public RT_Material
{
public:
    Metal(const glm::vec4& a, float f);
    ~Metal();

    virtual bool scatter(const Ray& r_in, const hit_record& rec, glm::vec4& attenuation, Ray& scattered) const;

public:
    glm::vec4 albedo;
    float fuzz;
};