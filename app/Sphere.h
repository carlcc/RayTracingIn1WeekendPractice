#pragma once

#include "Hitable.h"
#include "RTMaterial.h"

class Sphere : public Hitable {
public:
    Vec3f mCenter;
    float radius;
    RTMaterial* mMaterial;

    Sphere();

    Sphere(const Vec3f& center, float r, RTMaterial* material = nullptr) : mCenter(center), radius(r), mMaterial(material) { }

    bool hit(const Ray& r, float tMin, float tMax, HitRecord& rec) const override;

private:
};



