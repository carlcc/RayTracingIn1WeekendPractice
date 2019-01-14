/**
 * Copyright (c) 2018
 * Carl Chen. All rights reserved.
 *
 * This file is part of the project used to learn OpenGL.
 * Created by Chen Chen on 2018/12/23.
 */
#include <iostream>
#include "gl/Texture2D.h"
#include "BaseApplication.h"
#include "Paths.h"
#include "Image.h"
#include "FrameRateCounter.h"
#include "gl/ShaderProgram.h"
#include "TimeManager.h"
#include "InputManager.h"
#include "Vec3f.h"
#include "HitableList.h"
#include "Sphere.h"
#include "RTCamera.h"


class Antialiasing : public BaseApplication {
public:
    explicit Antialiasing(const std::string &title) :
            BaseApplication(title)
    {

    }

    ~Antialiasing()
    {

    }

    bool initialize() override;

    void finalize() override;

protected:

    bool prepareImage();

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

Vec3f color(const Ray& r, Hitable* world)
{
    HitRecord rec;
    if (world->hit(r, 0.0f, MAXFLOAT, rec)) {
        return 0.5f * Vec3f(rec.normal.x + 1, rec.normal.y + 1, rec.normal.z + 1);
    } else {
        Vec3f norm = r.getDirection();
        norm.normalize();
        float t = 0.5f * (norm.y + 1);
        return (1.0 - t) * Vec3f(1.f, 1.0f, 1.0f) + t*Vec3f(0.5f, 0.7f, 1.0f);
    }
}

bool Antialiasing::prepareImage()
{
    int nx = 800;
    int ny = 400;
    int ns = 4;

    mImage.reallocate(nx, ny);

    struct RGB {
        uint8_t r, g, b;
    };
    RGB *img = (RGB *) mImage.getData();

    Hitable* list[2] = {
            new Sphere(Vec3f(0.0f, 0.0f, -1.0f), 0.5),
            new Sphere(Vec3f(0.0f, -100.5f, -1.0f), 100)
    };
    Hitable* world = new HitableList(list, 2);

    RTCamera cam;
    float offsets[] = {
            0.25f, 0.25f, 0.25f, 0.75f,
            0.75f, 0.25f, 0.75f, 0.75f,
    };
    for (int j = ny-1; j >= 0; j--) {
        for (int i = 0; i < nx; ++i) {
            Vec3f col(0.0f);
            for (int k = 0; k < ns; ++k) {
                float u = 1.0f * (i+offsets[k*2]) / nx;
                float v = 1.0f * (j+offsets[k*2+1]) / ny;
                Ray r = cam.getRay(u, v);
                col += color(r, world);
            }
            col /= float(ns);

            img->r = (uint8_t) (col.r * 255);
            img->g = (uint8_t) (col.g * 255);
            img->b = (uint8_t) (col.b * 255);
            ++img;
        }
    }

    return true;
}

bool Antialiasing::initialize()
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
            {GL_VERTEX_SHADER,   CURRENT_DIRECTORY + "../../assets/shader/shader.vert", 0},
            {GL_FRAGMENT_SHADER, CURRENT_DIRECTORY + "../../assets/shader/shader.frag", 0},
            {GL_NONE,            "",                                                    0},
    };
    if (!mProgram.load(shaders)) {
        FATAL("Load shaders failed");
        return false;
    }


    mProgram.use();
    mProgram.setInt("ourTexture", 0);

    return true;
}

void Antialiasing::finalize()
{

}

void Antialiasing::display(bool autoRedraw)
{
    glClearColor(0.0f, 0.8f, 0.2f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    mProgram.use();
    mTexture.bind();
    glBindVertexArray(mVAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);

    BaseApplication::display(autoRedraw);
}

void Antialiasing::tick()
{
    mFrameRateCounter.count();
}

void Antialiasing::onMouseMove(int x, int y, int dx, int dy)
{
}

void Antialiasing::onMouseButton(int button, int action)
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

DEFINE_MAIN("Antialiasing", Antialiasing, nullptr, Trace);