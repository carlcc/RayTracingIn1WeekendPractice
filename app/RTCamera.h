#pragma once

#include "Vec3f.h"
#include "Ray.h"

class RTCamera {

    static Vec3f randomUnitVectorOnLens() {
        while (true) {
            Vec3f v((float) drand48(), (float) drand48(), (float) drand48());
            if (v.squaredLength() > 1.0f)
                continue;
            return v;
        }
    }
public:
    Vec3f origin;
    Vec3f lowerLeftCorner;
    Vec3f horizontal;
    Vec3f vertical;
    float lensRadius;
    Vec3f right;
    Vec3f up;
    Vec3f back;

    RTCamera(const Vec3f& origin, const Vec3f& target, const Vec3f& worldUp, float vFovDeg, float aspect, float focusDistance = 1.0f, float lensRadius = -1.0f)
    {
        back = origin - target;
        back.normalize();
        right = cross(worldUp, back);
        right.normalize();
        up = cross(back, right);
        up.normalize();

        float halfVert = std::tan(vFovDeg * float(M_PI) / 180.f / 2) * focusDistance;
        float halfHori = halfVert * aspect;

        this->lowerLeftCorner = origin - back*focusDistance - halfVert * up - halfHori * right;
        this->origin = origin;

        this->horizontal = 2 * halfHori * right;
        this->vertical = 2 * halfVert * up;
        this->lensRadius = lensRadius;
    }

    RTCamera() {
        lowerLeftCorner = Vec3f(-2.0f, -1.0f, -1.0f);
        horizontal = Vec3f(4.0f, 0.0f, 0.0f);
        vertical = Vec3f(0.0f, 2.0f, 0.0f);
        origin = Vec3f(0.0f);
    }

    Ray getRay(float u, float v) {
        Vec3f tmp;
        if (lensRadius > 0.0f) {
            Vec3f r = lensRadius * randomUnitVectorOnLens();
            tmp = r.x * right + r.y * up;
        } else {
            tmp = Vec3f(.0f);
        }
        return Ray(origin + tmp, lowerLeftCorner + u*horizontal + v*vertical - origin - tmp);
    }
};