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
    float blur;

    explicit RTMetalMaterial(const Vec3f& a, float blur = 0.0f) : albedo(a), blur(blur) { }

    virtual bool scatter(const Ray& ray, const HitRecord& rec, Vec3f& attenuation, Ray& scattered) const {
        Vec3f reflected = reflect(ray.getDirection(), rec.normal);
        if (blur > 0.0f) {
            reflected += randomInUnitSphere() * blur;
        }
        reflected.normalize();
        scattered = Ray(rec.p, reflected);
        attenuation = albedo;

        return (dot(scattered.getDirection(), rec.normal) > 0);
    }
};

class RTDielectricMaterial : public RTMaterial {
public:
    float refIdx;

    explicit RTDielectricMaterial(float ri) : refIdx(ri) { }

    virtual bool scatter(const Ray& ray, const HitRecord& rec, Vec3f& attenuation, Ray& scattered) const {
        Vec3f outNorm;

        float niOverNt;
        Vec3f refracted;
        float cosine;
        if (dot(ray.getDirection(), rec.normal) > 0) {
            outNorm = -rec.normal;
            niOverNt = refIdx;
            cosine = refIdx * dot(ray.getDirection(), rec.normal) / ray.getDirection().length();
        } else {
            outNorm = rec.normal;
            niOverNt = 1.0f / refIdx;
            cosine = -dot(ray.getDirection(), rec.normal) / ray.getDirection().length();
        }

        float reflectProb;
        attenuation = Vec3f(1.0f, 1.0f, 1.0f);
        if (refract(ray.getDirection(), outNorm, niOverNt, refracted)) {
            reflectProb = schlick(cosine, refIdx);
        } else {
            reflectProb = 1.0f;
//            return false;
        }
        if (drand48() < reflectProb) {
            Vec3f reflected = reflect(ray.getDirection(), rec.normal);
            reflected.normalize();
            scattered = Ray(rec.p, reflectProb);
        } else {
            scattered = Ray(rec.p, refracted);
        }
        return true;

    }

private:
    static float schlick(float cosine, float refIndex) {
        float r0 = (1-refIndex) / (1+refIndex);
        r0 *= r0;
        return r0 + (1-r0)*std::pow(1.0f-cosine, 5.f);
    }
};