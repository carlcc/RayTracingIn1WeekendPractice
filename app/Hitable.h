#pragma once

#include "Ray.h"

class RTMaterial;

struct HitRecord {
    float t;
    Vec3f p;
    Vec3f normal;

    RTMaterial* material;
};

class Hitable {
public:
    virtual bool hit(const Ray& r, float tMin, float tMax, HitRecord& rec) const = 0;
};