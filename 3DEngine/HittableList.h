#pragma once
#include "IHittable.h"
#include <vector>
#include <memory>

class HittableList : public IHittable
{
public:
    HittableList();
    HittableList(std::shared_ptr<IHittable> object);
    ~HittableList();

    void clear();
    void add(std::shared_ptr<IHittable> object);
    virtual bool hit(const Ray& r, float tmin, float tmax, hit_record& rec) const;

    std::vector<std::shared_ptr<IHittable>> objects;
};

