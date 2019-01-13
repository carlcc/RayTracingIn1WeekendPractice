/**
 * Copyright (c) 2018
 * Carl Chen. All rights reserved.
 *
 * This file is part of MSServer.
 * Created by Chen Chen on 2018/12/20.
 */

#include "WorldGridRenderer.h"
#include "Paths.h"
#include "Log.h"

WorldGridRenderer::WorldGridRenderer(): mVAO(0), mVBO(0), mTotalPointsCount(0), mGridSize(1.0f), mProgram(), mCamera(nullptr)
{

}

WorldGridRenderer::~WorldGridRenderer()
{
    if (mVAO != 0) {
        glDeleteVertexArrays(1, &mVAO);
    }
    if (mVBO != 0) {
        glDeleteBuffers(1, &mVBO);
    }

}

bool WorldGridRenderer::initialize(const Camera* camera, int gridCount)
{
    mCamera = camera;
    int lineCount = gridCount * 2 + 1;
    mTotalPointsCount = static_cast<uint32_t>(lineCount * 2 * 2 + 1*2);       // +1 y axis
    float minPos = -gridCount;
    float maxPos = -minPos;
    auto* points = new GLfloat[mTotalPointsCount * 3];      // ((gridCount * 2 + 1) * 2 + 1) * 2 * 3

    int k = 0;
    for (int i = 0; i < lineCount; ++i) {
        points[k++] = minPos;         // x
        points[k++] = 0.0f;           // y
        points[k++] = minPos + i;     // z
        points[k++] = maxPos;         // x
        points[k++] = 0.0f;           // y
        points[k++] = minPos + i;     // z
    }
    points[lineCount/2 * 6] = minPos * 2;
    points[lineCount/2 * 6 + 3] = maxPos * 2;

    for (int i = 0; i < lineCount; ++i) {
        points[k++] = minPos + i;         // x
        points[k++] = 0.0f;           // y
        points[k++] = minPos;     // z
        points[k++] = minPos + i;         // x
        points[k++] = 0.0f;           // y
        points[k++] = maxPos;     // z
    }
    points[lineCount*6 + lineCount/2 * 6 + 2] = minPos * 2;
    points[lineCount*6 + lineCount/2 * 6 + 5] = maxPos * 2;

    points[k++] = 0.0f;         // x
    points[k++] = minPos * 2;     // y
    points[k++] = 0.0f;           // z
    points[k++] = 0.0f;         // x
    points[k++] = maxPos * 2;     // y
    points[k++] = 0.0f;           // z

    ASSERT(points + k == points + mTotalPointsCount * 3, "Index out of bound!");

    glGenVertexArrays(1, &mVAO);
    glGenBuffers(1, &mVBO);

    glBindVertexArray(mVAO);
    glBindBuffer(GL_ARRAY_BUFFER, mVBO);

    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * mTotalPointsCount * 3, points, GL_STATIC_DRAW);
    delete[] points;

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    ShaderInfo2 shaderInfos[] = {
            {
                GL_VERTEX_SHADER,
                "#version 330 core\n"
                "layout (location = 0) in vec3 aPos;\n"
                "\n"
                "uniform mat4 project;\n"
                "uniform mat4 view;\n"
                "uniform vec3 scale;\n"
                "out vec4 Color;\n"
                "void main()\n"
                "{\n"
                "    gl_Position = project * view * (vec4(scale, 1.0) * vec4(aPos, 1.0));\n"
                "   if (aPos.y == 0 && aPos.z == 0) { if (aPos.x > 0) Color = vec4(1.0f, 0.0f, 0.0f, 1.0f); else Color = vec4(1.0f); }\n"
                "   else if (aPos.x == 0 && aPos.z == 0 && aPos.y > 0) { if (aPos.y > 0) Color = vec4(0.0f, 1.0f, 0.0f, 1.0f); else Color = vec4(1.0f); }\n"
                "   else if (aPos.y == 0 && aPos.x == 0 && aPos.z > 0) { if (aPos.z > 0) Color = vec4(0.0f, 0.0f, 1.0f, 1.0f); else Color = vec4(1.0f); }\n"
                "   else { Color = vec4(1.0f); }\n"
                "}",
                0
            }, {
                GL_FRAGMENT_SHADER,
                "#version 330 core\n"
                "out vec4 FragColor;\n"
                "in vec4 Color;\n"
                "\n"
                "void main()\n"
                "{\n"
                "    FragColor = Color;\n"
                "}",
                0
            }, { GL_NONE, "", 0},
    };
    return mProgram.load(shaderInfos);
}

void WorldGridRenderer::render()
{
    mProgram.use();
    mProgram.setMat4("view", mCamera->getViewMatrix());
    mProgram.setMat4("project", mCamera->getProjectionMatrix());
    mProgram.setVec3("scale", glm::vec3(mGridSize));
    glBindVertexArray(mVAO);
    glDrawArrays(GL_LINES, 0, mTotalPointsCount);
}

void WorldGridRenderer::setGridSize(float size)
{
    if (size <= 0.0f) {
        mGridSize = 0.1f;
    } else {
        mGridSize = size;
    }
}
