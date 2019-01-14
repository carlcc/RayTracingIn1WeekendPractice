//
// Created by carl on 19-1-13.
//
#pragma once

#include "Vec3f.h"

class Ray {
public:
    Ray() { }
    Ray(const Vec3f& origin, const Vec3f& dir): mOrigin(origin), mDir(dir) {
    }

    Vec3f getOrigin() const {
        return mOrigin;
    }

    Vec3f getDirection() const {
        return mDir;
    }

    Vec3f getPointAtT(float t) const {
        return mOrigin + mDir * t;
    }


private:
    Vec3f mOrigin;
    Vec3f mDir;
};
