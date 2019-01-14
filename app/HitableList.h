#pragma once

#include "Hitable.h"

/**
 * HitableList is a class represents
 */
class HitableList : public Hitable {
public:
    HitableList();
    HitableList(Hitable** l, int n) {
        mList = l;
        mSize = n;
    }

    bool hit(const Ray& r, float tMin, float tMax, HitRecord& rec) const override;

private:
    Hitable** mList;
    int mSize;
};



