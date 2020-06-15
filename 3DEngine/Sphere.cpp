#include "Sphere.h"

Sphere::Sphere(glm::vec3 cen, float r, std::shared_ptr<RT_Material> m) : center(cen), radius(r), mat_ptr(m)
{
}

Sphere::~Sphere()
{
}

bool Sphere::hit(const Ray & r, float t_min, float t_max, hit_record & rec) const
{
    glm::vec3 oc = r.origin() - center;
    float a = glm::length2(r.direction());
    float b = 2.0 * dot(oc, r.direction());
    float c = glm::length2(oc) - radius * radius;
    float discriminant = b * b - 4 * a*c;

    if (discriminant > 0) {
        float root = sqrt(discriminant);
        float temp = (-b - root) / (2*a);
        if (temp < t_max && temp > t_min) {
            rec.t = temp;
            rec.position = r.at(rec.t);
            rec.normal = (rec.position - center) / radius;
            glm::vec3 outward_normal = (rec.position - center) / radius;
            rec.set_face_normal(r, outward_normal);
            rec.mat_ptr = mat_ptr;
            return true;
        }
        temp = (-b + root) / (2*a);
        if (temp < t_max && temp > t_min) {
            rec.t = temp;
            rec.position = r.at(rec.t);
            rec.normal = (rec.position - center) / radius;
            glm::vec3 outward_normal = (rec.position - center) / radius;
            rec.set_face_normal(r, outward_normal);
            rec.mat_ptr = mat_ptr;
            return true;
        }
    }
    return false;
}
