#include "Metal.h"

Metal::Metal(const glm::vec4 & a, float f): albedo(a), fuzz(f < 1 ? f : 1)
{
}

Metal::~Metal()
{
}

glm::vec3 reflect(const glm::vec3 & v, const glm::vec3 & n) {
    return v - 2 * glm::dot(v, n)*n;
}

bool Metal::scatter(const Ray & r_in, const hit_record & rec, glm::vec4 & attenuation, Ray & scattered) const {
    glm::vec3 reflected = reflect(glm::normalize(r_in.direction()), rec.normal);
    scattered = Ray(rec.position, reflected);
    scattered = Ray(rec.position, reflected + fuzz * RayTracingScene::getRandomInUnitSphere());

    attenuation = albedo;
    return (dot(scattered.direction(), rec.normal) > 0);
}
