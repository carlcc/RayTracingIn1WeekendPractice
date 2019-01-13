/**
 * Copyright (c) 2018
 * Carl Chen. All rights reserved.
 *
 * This file is part of the project used to learn OpenGL.
 * Created by Chen Chen on 2018/12/23.
 */
#include <iostream>

#include "gl/Texture2D.h"
#include "gl/ShaderProgram.h"
#include "Ray.h"
#include "BaseApplication.h"
#include "Paths.h"
#include "Image.h"
#include "FrameRateCounter.h"
#include "TimeManager.h"
#include "InputManager.h"


class Program1 : public BaseApplication {
public:
    explicit Program1(const std::string &title) :
            BaseApplication(title)
    {

    }

    ~Program1()
    {

    }

    bool prepareImage();

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

Vec3f color(const Ray& r)
{
    Vec3f dir = r.getDirection();
    dir.normalize();

    float t = (dir.y + 1.0f) * 0.5f;

    return (1.0-t)*Vec3f(1.0f) + Vec3f(0.5f, 0.7f, 1.0f)*t;
}
bool Program1::prepareImage()
{
    int nx = 200;
    int ny = 100;

    mImage.reallocate(200, 100);

    struct RGB {
        uint8_t r, g, b;
    };
//    Vec3f (*img)[200][100] = (Vec3f(*)[200][100]) mImage.getData();
    RGB *img = (RGB *) mImage.getData();

    Vec3f lowerLeftCorner(-2.0f, -1.0f, -1.0f);
    Vec3f horizontal(4.0f, 0.0f, 0.0f);
    Vec3f vertical(0.0f, 2.0f, 0.0f);
    Vec3f origin(0.0f, 0.0f, 0.0f);

    int k = 0;
    for (int j = ny-1; j >= 0; j--) {
        for (int i = 0; i < nx; ++i) {
            float u = 1.0f * i / nx;
            float v = 1.0f * j / ny;
            Ray r(origin, lowerLeftCorner + horizontal*u + vertical*v);
            Vec3f col = color(r);

            img->r = (uint8_t) (col.r * 255);
            img->g = (uint8_t) (col.g * 255);
            img->b = (uint8_t) (col.b * 255);
            ++img;
        }
    }

    return true;
}

bool Program1::initialize()
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

    prepareImage();

    Texture2D::active(GL_TEXTURE0);
    mTexture.initialize();
    mTexture.bufferImage(mImage.getData(), mImage.getWidth(), mImage.getHeight(), Texture2D::RGB888);

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
    mProgram.setInt("ourTexture", 0);

    return true;
}

void Program1::finalize()
{

}

void Program1::display(bool autoRedraw)
{
    glClearColor(0.0f, 0.8f, 0.2f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    mProgram.use();
    mTexture.bind();
    glBindVertexArray(mVAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);

    BaseApplication::display(autoRedraw);
}

void Program1::tick()
{
    mFrameRateCounter.count();
}

void Program1::onMouseMove(int x, int y, int dx, int dy)
{
}

void Program1::onMouseButton(int button, int action)
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

DEFINE_MAIN("Program1", Program1, nullptr, Trace);