/**
 * Copyright (c) 2018 Duobei Brothers Information Technology Co.,Ltd. All rights reserved.
 * This file is part of Janus-gateway. 
 * Specifically, it is part of Duobei Streaming Plugin.
 *
 * Author: Chen Chen (chen.chen@duobei.com)
 * 
 * Date: 2019-1-14
 *
 * About this file: This file holds the declaration/implementation of class HitableList
 */

#include "HitableList.h"

HitableList::HitableList()
{

}

bool HitableList::hit(const Ray& r, float tMin, float tMax, HitRecord& rec) const
{
    HitRecord tmpRec;
    bool hasHitAnything = false;
    float closestSoFar = tMax;
    for (int i = 0; i < mSize; ++i) {
        if (mList[i]->hit(r, tMin, closestSoFar, tmpRec)) {
            hasHitAnything = true;
            closestSoFar = tmpRec.t;
            rec = tmpRec;
        }
    }
    return hasHitAnything;
}
