/**
 * Copyright (c) 2018
 * Carl Chen. All rights reserved.
 *
 * This file is part of MSServer.
 * Created by Chen Chen on 2018/12/20.
 */

#pragma once


#include <glad/glad.h>
#include "gl/ShaderProgram.h"
#include "Camera.h"

class WorldGridRenderer {
public:
    WorldGridRenderer();
    ~WorldGridRenderer();

    bool initialize(const Camera* camera, int gridCount);

    void setGridSize(float size);

    void render();

private:
    GLuint mVAO;
    GLuint mVBO;
    uint32_t mTotalPointsCount;
    float mGridSize;
    ShaderProgram mProgram;
    const Camera* mCamera;
};



