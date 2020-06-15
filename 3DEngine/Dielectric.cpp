#include "Dielectric.h"

Dielectric::Dielectric(double ri) : ref_idx(ri)
{
}

Dielectric::~Dielectric()
{
}

double schlick(double cosine, double ref_idx) {
    auto r0 = (1 - ref_idx) / (1 + ref_idx);
    r0 = r0 * r0;
    return r0 + (1 - r0)*pow((1 - cosine), 5);
}

bool Dielectric::scatter(const Ray & r_in, const hit_record & rec, glm::vec4 & attenuation, Ray & scattered) const {
    attenuation = glm::vec4(1.0, 1.0, 1.0, 1.0);
    float etai_over_etat;
    if (rec.front_face) {
        etai_over_etat = 1.0 / ref_idx;
    }
    else {
        etai_over_etat = ref_idx;
    }

    glm::vec3 unit_direction = glm::normalize(r_in.direction());

    double cos_theta = fmin(dot(-unit_direction, rec.normal), 1.0);
    double sin_theta = sqrt(1.0 - cos_theta * cos_theta);
    if (etai_over_etat * sin_theta > 1.0) {
        glm::vec3 reflected = glm::reflect(unit_direction, rec.normal);
        scattered = Ray(rec.position, reflected);
        return true;
    }
    double reflect_prob = schlick(cos_theta, etai_over_etat);
    if (RayTracingScene::getRandomFloat() < reflect_prob)
    {
        glm::vec3 reflected = reflect(unit_direction, rec.normal);
        scattered = Ray(rec.position, reflected);
        return true;
    }
    glm::vec3 refracted = glm::refract(unit_direction, rec.normal, etai_over_etat);
    scattered = Ray(rec.position, refracted);
    return true;
}
