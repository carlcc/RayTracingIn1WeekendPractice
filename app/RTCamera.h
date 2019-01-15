#pragma once

#include "Vec3f.h"
#include "Ray.h"

class RTCamera {
public:
    Vec3f origin;
    Vec3f lowerLeftCorner;
    Vec3f horizontal;
    Vec3f vertical;

    RTCamera(const Vec3f& origin, const Vec3f& target, const Vec3f& worldUp, float vFovDeg, float aspect)
    {
        Vec3f zDir = origin - target;
        zDir.normalize();
        Vec3f xDir = cross(worldUp, zDir);
        xDir.normalize();
        Vec3f yDir = cross(zDir, xDir);
        yDir.normalize();

        float halfVert = std::tan(vFovDeg * float(M_PI) / 180.f / 2);
        float halfHori = halfVert * aspect;

        this->lowerLeftCorner = origin - zDir - halfVert * yDir - halfHori * xDir;
        this->origin = origin;

        this->horizontal = 2 * halfHori * xDir;
        this->vertical = 2 * halfVert * yDir;
    }

    RTCamera() {
        lowerLeftCorner = Vec3f(-2.0f, -1.0f, -1.0f);
        horizontal = Vec3f(4.0f, 0.0f, 0.0f);
        vertical = Vec3f(0.0f, 2.0f, 0.0f);
        origin = Vec3f(0.0f);
    }

    Ray getRay(float u, float v) {
        return Ray(origin, lowerLeftCorner + u*horizontal + v*vertical - origin);
    }
};