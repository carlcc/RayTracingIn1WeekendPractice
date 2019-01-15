/**
 * Copyright (c) 2018 Duobei Brothers Information Technology Co.,Ltd. All rights reserved.
 * This file is part of Janus-gateway. 
 * Specifically, it is part of Duobei Streaming Plugin.
 *
 * Author: Chen Chen (chen.chen@duobei.com)
 * 
 * Date: 2019-1-14
 *
 * About this file: This file holds the declaration/implementation of class Sphere
 */

#include "Sphere.h"

Sphere::Sphere()
{

}

bool Sphere::hit(const Ray& r, float tMin, float tMax, HitRecord& rec) const
{
    Vec3f oc = r.getOrigin() - mCenter;
    float a = dot(r.getDirection(), r.getDirection());
    float b = dot(oc, r.getDirection());
    float c = dot(oc, oc) - radius*radius;
    float discriminant = b*b - a*c;

    if (discriminant > 0) {
        float tmp = (-b - std::sqrt(b*b-a*c)) / a;
        if (tmp < tMax && tmp > tMin) {
            rec.t = tmp;
            rec.p = r.getPointAtT(rec.t);
            rec.normal = (rec.p - mCenter) / radius;
            rec.material = mMaterial;
            return true;
        }
        tmp = (-b + std::sqrt(b*b-a*c)) / a;
        if (tmp < tMax && tmp > tMin) {
            rec.t = tmp;
            rec.p = r.getPointAtT(rec.t);
            rec.normal = (rec.p - mCenter) / radius;
            rec.material = mMaterial;
            return true;
        }
    }
    return false;
}