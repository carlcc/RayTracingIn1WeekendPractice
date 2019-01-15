#pragma once

#include "Ray.h"
#include "Hitable.h"

class RTMaterial {
protected:
    static Vec3f randomInUnitSphere()
    {
        while (true) {
            Vec3f p = 2.0f*Vec3f(drand48(),drand48(),drand48()) - Vec3f(1.0f);
            if (p.squaredLength() < 1.0f) {
                return p;
            }
        }
    }
public:
    virtual bool scatter(const Ray& ray, const HitRecord& rec, Vec3f& attenuation, Ray& scattered) const = 0;
};

class RTLambertianMaterial : public RTMaterial {
public:
    Vec3f albedo;

    explicit RTLambertianMaterial(const Vec3f& a) : albedo(a) { }
    virtual bool scatter(const Ray& ray, const HitRecord& rec, Vec3f& attenuation, Ray& scattered) const {
        Vec3f target = rec.p + rec.normal + randomInUnitSphere();
        scattered = Ray(rec.p, target-rec.p);
        attenuation = albedo;
        return true;
    }
};

class RTMetalMaterial : public RTMaterial {
public:
    Vec3f albedo;

    explicit RTMetalMaterial(const Vec3f& a) : albedo(a) { }

    virtual bool scatter(const Ray& ray, const HitRecord& rec, Vec3f& attenuation, Ray& scattered) const {
        Vec3f reflected = reflect(ray.getDirection(), rec.normal);
        reflected.normalize();
        scattered = Ray(rec.p, reflected);
        attenuation = albedo;

        return (dot(scattered.getDirection(), rec.normal) > 0);
    }
};