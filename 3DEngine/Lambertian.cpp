#include "Lambertian.h"
#include "RayTracingScene.h"

Lambertian::Lambertian(const glm::vec4 & a) : albedo(a)
{
}

Lambertian::~Lambertian()
{
}

bool Lambertian::scatter(const Ray & r_in, const hit_record & rec, glm::vec4 & attenuation, Ray & scattered) const {
    glm::vec3 scatter_direction = rec.normal + RayTracingScene::getRandomUnitVector();
    scattered = Ray(rec.position, scatter_direction);
    attenuation = albedo;
    return true;
}
