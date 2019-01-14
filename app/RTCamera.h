#pragma once

#include "Vec3f.h"
#include "Ray.h"

class RTCamera {
public:
    Vec3f origin;
    Vec3f lowerLeftCorner;
    Vec3f horizontal;
    Vec3f vertical;


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