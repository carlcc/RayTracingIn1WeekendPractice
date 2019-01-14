#pragma once

#include "Hitable.h"

class Sphere : public Hitable {
public:
    Vec3f mCenter;
    float radius;

    Sphere();

    Sphere(const Vec3f& center, float r) : mCenter(center), radius(r) { }

    bool hit(const Ray& r, float tMin, float tMax, HitRecord& rec) const override;

private:

};



