/**
 * Copyright (c) 2018
 * Carl Chen. All rights reserved.
 *
 * This file is part of the project used to learn OpenGL.
 * Created by Chen Chen on 2018/12/23.
 */
#include <iostream>
#include <queue>
#include <mutex>
#include "gl/Texture2D.h"
#include "BaseApplication.h"
#include "Paths.h"
#include "Image.h"
#include "FrameRateCounter.h"
#include "gl/ShaderProgram.h"
#include "TimeManager.h"
#include "InputManager.h"
#include "Sphere.h"
#include "HitableList.h"
#include "RTCamera.h"
#include "RTMaterial.h"

class OGLRT : public BaseApplication {
public:
    explicit OGLRT(const std::string &title) :
            BaseApplication(title)
    {

    }

    ~OGLRT()
    {

    }

    bool initialize() override;

    void finalize() override;

protected:

    void display(bool autoRedraw) override;

    void onResize(int width, int height) override
    {
        BaseApplication::onResize(width, height);
    }

    void tick() override;

    void onMouseMove(int x, int y, int dx, int dy) override;

    void onMouseButton(int button, int action) override;

    void onKey(int key, int scancode, int action, int mods) override
    {
        if (key == SDLK_ESCAPE && action == SDL_KEYUP) {
            setIsRunning(false);
        }
        BaseApplication::onKey(key, scancode, action, mods);
    }

    void onChar(unsigned int codepoint) override
    {
        BaseApplication::onChar(codepoint);
    }

public:
    bool mLeftButtonPressed{false};
    bool mRightButtonPressed{false};
    bool mPause{false};
    FrameRateCounter mFrameRateCounter;
    Image mImage;

    ShaderProgram mProgram;
    GLuint mVBO, mVAO;
    Texture2D mTexture;
};

bool OGLRT::initialize()
{
    if (!BaseApplication::initialize()) {
        return false;
    }

    mFrameRateCounter.reset();

    // Add you code here
    glGenVertexArrays(1, &mVAO);
    glGenBuffers(1, &mVBO);
    glBindVertexArray(mVAO);
    glBindBuffer(GL_ARRAY_BUFFER, mVBO);

    float vertices[] = {
            // positions          // colors           // texture coords
            1.f, 1.f, 0.0f, 1.0f, 1.0f, // top right
            1.f, -1.f, 0.0f, 1.0f, 0.0f, // bottom right
            -1.f, 1.f, 0.0f, 0.0f, 1.0f,  // top left

            1.f, -1.f, 0.0f, 1.0f, 0.0f, // bottom right
            -1.f, -1.f, 0.0f, 0.0f, 0.0f, // bottom left
            -1.f, 1.f, 0.0f, 0.0f, 1.0f  // top left
    };
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (void *) 0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (void *) (3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);

//    prepareImage();
//
//    Texture2D::active(GL_TEXTURE0);
//    mTexture.initialize();
//    mTexture.bufferImage(mImage.getData(), mImage.getWidth(), mImage.getHeight(), Texture2D::RGB888);

    ShaderInfo shaders[] = {
            {GL_VERTEX_SHADER,   CURRENT_DIRECTORY + "shader.vert", 0},
            {GL_FRAGMENT_SHADER, CURRENT_DIRECTORY + "shader.frag", 0},
            {GL_NONE,            "",                                0},
    };
    if (!mProgram.load(shaders)) {
        FATAL("Load shaders failed");
        return false;
    }


    mProgram.use();


    return true;
}

void OGLRT::finalize()
{

}

const static int MAT_GLASS = 0;
const static int MAT_METAL = 1;
const static int MAT_DIFFUSE = 3;
void OGLRT::display(bool autoRedraw)
{
    glClearColor(0.0f, 0.8f, 0.2f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    mProgram.use();

    RTCamera cam;

    mProgram.setVec3("gCamera.origin", cam.origin.data);
    mProgram.setVec3("gCamera.lowerLeftCorner", cam.lowerLeftCorner.data);
    mProgram.setVec3("gCamera.horizontal", cam.horizontal.data);
    mProgram.setVec3("gCamera.vertical", cam.vertical.data);
    mProgram.setFloat("gCamera.lensRadius", cam.lensRadius);
    mProgram.setVec3("gCamera.right", cam.right.data);
    mProgram.setVec3("gCamera.up", cam.up.data);
    mProgram.setVec3("gCamera.back", cam.back.data);
    mProgram.setInt("viewPortWidth", getWidth());
    mProgram.setInt("viewPortHeight", getHeight());

    mProgram.setVec3("gWorld[0].center", Vec3f(0.0f, 0.0f, -1.0f).data);
    mProgram.setFloat("gWorld[0].radius", 0.5);
    mProgram.setInt("gWorld[0].material", 0);
    mProgram.setVec3("gWorld[1].center", Vec3f(0.0f, -100.5f, -1.0f).data);
    mProgram.setFloat("gWorld[1].radius", 100.f);
    mProgram.setInt("gWorld[1].material", 1);

    mProgram.setInt("gMaterials[0].type", MAT_DIFFUSE);
    mProgram.setVec3("gMaterials[0].color", Vec3f(1.f, .0f, .0f).data);
    mProgram.setFloat("gMaterials[0].fractionRate", 1.5f);
    mProgram.setInt("gMaterials[1].type", MAT_METAL);
    mProgram.setVec3("gMaterials[1].color", Vec3f(.5f, .5f, .5f).data);
    mProgram.setFloat("gMaterials[1].fractionRate", 1.5f);

    mProgram.setInt("gNSpheres", 2);


    glBindVertexArray(mVAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);

    BaseApplication::display(autoRedraw);
}

void OGLRT::tick()
{
    mFrameRateCounter.count();
}

void OGLRT::onMouseMove(int x, int y, int dx, int dy)
{
}

void OGLRT::onMouseButton(int button, int action)
{
    if (button == SDL_BUTTON_LEFT) {
        if (action == SDL_MOUSEBUTTONDOWN) {
            mLeftButtonPressed = true;
            showMouseCursor(false);
        } else if (action == SDL_MOUSEBUTTONUP) {
            mLeftButtonPressed = false;
            showMouseCursor(true);
        }
    }
    if (button == SDL_BUTTON_RIGHT) {
        if (action == SDL_MOUSEBUTTONDOWN) {
            showMouseCursor(false);
            mRightButtonPressed = true;
        } else if (action == SDL_MOUSEBUTTONUP) {
            mRightButtonPressed = false;
            showMouseCursor(true);
        }
    }
}

DEFINE_MAIN("OGLRT", OGLRT, nullptr, Trace);