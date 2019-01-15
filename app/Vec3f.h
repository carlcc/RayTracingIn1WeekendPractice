//
// Created by carl on 19-1-13.
//

#pragma once

#include <cmath>
#include <cstdlib>
#include <iostream>
#include "Log.h"


class Vec3f {
public:
    Vec3f() { }
    Vec3f(float a) : x(a), y(a), z(a) {}
    Vec3f(float a, float b, float c) : x(a), y(b), z(c) {}
    Vec3f(const Vec3f& v): x(v.x), y(v.y), z(v.z) { }

    inline const Vec3f& operator+() const { return *this; }
    inline Vec3f operator-() const { return Vec3f(-x, -y, -z); }
    inline float operator[](int i) const {
        ASSERT(i >=0 && i < 4, "Index out of bounds");
        return data[i];
    }
    inline float& operator[](int i) {
        ASSERT(i >=0 && i < 4, "Index out of bounds");
        return data[i];
    }

    inline Vec3f& operator+=(const Vec3f& v) {
        x += v.x;
        y += v.y;
        z += v.z;
        return *this;
    }

    inline Vec3f& operator-=(const Vec3f& v) {
        x -= v.x;
        y -= v.y;
        z -= v.z;
        return *this;
    }

    inline Vec3f& operator*=(const Vec3f& v) {
        x *= v.x;
        y *= v.y;
        z *= v.z;
        return *this;
    }

    inline Vec3f& operator/=(const Vec3f& v) {
        x /= v.x;
        y /= v.y;
        z /= v.z;
        return *this;
    }

    inline Vec3f& operator*=(float s) {
        x *= s;
        y *= s;
        z *= s;
        return *this;
    }

    inline Vec3f& operator/=(float s) {
        x /= s;
        y /= s;
        z /= s;
        return *this;
    }

    inline float squaredLength() const {
        return x*x + y*y + z*z;
    }
    inline float length() const {
        return std::sqrt(x*x + y*y + z*z);
    }

    inline void normalize() {
        *this /= length();
    }

public:
    union {
        float data[3];
        struct {
            float x;
            float y;
            float z;
        };
        struct {
            float r;
            float g;
            float b;
        };
    };
};

inline std::istream& operator>>(std::istream& is, Vec3f& v)
{
    is >> v.x >> v.y >> v.z;
    return is;
}

inline std::ostream& operator<<(std::ostream& os, const Vec3f& v)
{
    os << '(' << v.x << ',' << v.y << ',' << v.z << ')';
    return os;
}

inline Vec3f operator+(const Vec3f& v1, const Vec3f& v2)
{
    return Vec3f(v1.x+v2.x, v1.y + v2.y, v1.z + v2.z);
}

inline Vec3f operator-(const Vec3f& v1, const Vec3f& v2)
{
    return Vec3f(v1.x-v2.x, v1.y - v2.y, v1.z - v2.z);
}

inline Vec3f operator*(const Vec3f& v1, const Vec3f& v2)
{
    return Vec3f(v1.x*v2.x, v1.y * v2.y, v1.z * v2.z);
}

inline Vec3f operator/(const Vec3f& v1, const Vec3f& v2)
{
    return Vec3f(v1.x/v2.x, v1.y / v2.y, v1.z / v2.z);
}

inline Vec3f operator*(const Vec3f& v1, float s)
{
    return Vec3f(v1.x * s, v1.y *s, v1.z *s);
}

inline Vec3f operator/(const Vec3f& v1, float s)
{
    return Vec3f(v1.x / s, v1.y /s, v1.z /s);
}

inline float dot(const Vec3f& v1, const Vec3f& v2)
{
    return v1.x*v2.x + v1.y * v2.y + v1.z * v2.z;
}

inline Vec3f cross(const Vec3f& v1, const Vec3f& v2)
{
    return Vec3f(
            v1.y * v2.z - v1.z * v2.y,
            v1.z * v2.x - v1.x * v2.z,
            v1.x * v2.y - v1.y * v2.x
    );
}

inline Vec3f reflect(const Vec3f& v, const Vec3f& n)
{
    return v - 2*dot(v,n)*n;
}

inline bool refract(const Vec3f& v, const Vec3f& n, float niOverNt, Vec3f& refracted)
{
    Vec3f uv = v;
    uv.normalize();
    float dt = dot(uv, n);
    float discriminant = 1.0f - niOverNt * niOverNt*(1-dt*dt);
    if (discriminant > 0) {
        refracted = niOverNt*(uv - n*dt) - n*std::sqrt(discriminant);
        return true;
    }
    return false;
}