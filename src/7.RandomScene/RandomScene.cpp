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

class RandomScene : public BaseApplication {
public:
    explicit RandomScene(const std::string &title) :
            BaseApplication(title)
    {

    }

    ~RandomScene()
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

Vec3f randomInUnitSphere()
{
    while (true) {
        Vec3f p = 2.0f*Vec3f(drand48(),drand48(),drand48()) - Vec3f(1.0f);
        if (p.squaredLength() < 1.0f) {
            return p;
        }
    }
}

Vec3f color(const Ray& r, Hitable* world, int depth)
{
    HitRecord rec;
    if (world->hit(r, 0.001f, 9999999.0f, rec)) {
        Ray scattered;
        Vec3f attenuation;
        if (depth > 0 && rec.material->scatter(r, rec, attenuation, scattered)) {
            return attenuation * color(scattered, world, depth - 1);
        }
        return Vec3f(0.0f);
    } else {
        Vec3f norm = r.getDirection();
        norm.normalize();
        float t = 0.5f * (norm.y + 1);
        return (1.0 - t) * Vec3f(1.f, 1.0f, 1.0f) + t*Vec3f(0.5f, 0.7f, 1.0f);
    }
}

Hitable* randomScene()
{
    Hitable** list = new Hitable*[240];
    int i = 0;
    for (i = 0; i < 100; ++i) {
        Vec3f center(drand48() * 16.0 - 8.0, 0.2f, drand48() * 16.0 - 8.0);
        int ballType = rand() % 6;

        if (ballType < 3) {
            Vec3f color(drand48(), drand48(), drand48());
            list[i] = new Sphere(center, 0.2f, new RTLambertianMaterial(color));
        } else if (ballType < 5) {
            Vec3f color(drand48(), drand48(), drand48());
            list[i] = new Sphere(center, 0.2f, new RTMetalMaterial(color, drand48() / 10.f));
        } else {
            list[i] = new Sphere(center, 0.2f, new RTDielectricMaterial(1.5f));
        }
    }
    list[i++] = new Sphere(Vec3f(0.0f, -1000.0f, 0.0), 1000.f, new RTLambertianMaterial(Vec3f(.5f, .5f, .5f)));
    list[i++] = new Sphere(Vec3f(0.0f, 1.0f, 0.0), 1.f, new RTLambertianMaterial(Vec3f(.4f, .2f, .1f)));
    list[i++] = new Sphere(Vec3f(2.0f, 1.0f, 0.0), 1.f, new RTDielectricMaterial(1.5f));
    list[i++] = new Sphere(Vec3f(-2.0f, 1.0f, 0.0), 1.f, new RTMetalMaterial(Vec3f(.7f, .6f, .5f)));

    return new HitableList(list, i);
}

const int nx = 1920;
const int ny = 1080;
const int ns = 200;
bool RandomScene::prepareImage()
{
    mImage.reallocate(nx, ny);

    struct RGB {
        uint8_t r, g, b;
    };
    struct Task {
        RGB* ptr;
        float u, v;
    };
    RGB *img = (RGB *) mImage.getData();

    Hitable* world = randomScene();

    std::queue<Task> tasks;
    std::mutex mutex;

    Vec3f lookFrom = Vec3f(-3.f, 1.f, -5.f);
    Vec3f lookAt = Vec3f(0.0f, 1.0f, 0.0);
    float focusDistance = (lookAt - lookFrom).length();
    RTCamera cam(lookFrom, lookAt, Vec3f(0.0f, 1.0f, 0.0f), 45, (float)nx / ny, focusDistance, .20f);
    for (int j = ny-1; j >= 0; j--) {
        for (int i = 0; i < nx; ++i) {
            float u = 1.0f * (i+drand48()) / nx;
            float v = 1.0f * (j+drand48()) / ny;
            tasks.push(Task { ++img, u, v });
//            ++img;
        }
    }

    std::thread threads[8];
    for (auto& i : threads) {
        i = std::thread([&tasks, &mutex, &cam, &world]() {
            while (true) {
                Task task;
                {
                    std::unique_lock<std::mutex> lck(mutex);
                    if (tasks.empty()) {
                        break;
                    }
                    task = tasks.front();
                    tasks.pop();
                }

                Vec3f col(0.0f);
                for (int k = 0; k < ns; ++k) {
                    Ray r = cam.getRay(task.u, task.v);
                    col += color(r, world, 50);
                }
                col /= float(ns);

                // gamma correction
                col = Vec3f(std::sqrt(col.x), std::sqrt(col.y), std::sqrt(col.z));
                task.ptr->r = (uint8_t) (col.r * 255);
                task.ptr->g = (uint8_t) (col.g * 255);
                task.ptr->b = (uint8_t) (col.b * 255);
            }
        });
    }
    for (auto& i : threads) {
        i.join();
    }

    return true;
}

bool RandomScene::initialize()
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

void RandomScene::finalize()
{

}

void RandomScene::display(bool autoRedraw)
{
    glClearColor(0.0f, 0.8f, 0.2f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    mProgram.use();
    mTexture.bind();
    glBindVertexArray(mVAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);

    BaseApplication::display(autoRedraw);
}

void RandomScene::tick()
{
    mFrameRateCounter.count();
}

void RandomScene::onMouseMove(int x, int y, int dx, int dy)
{
}

void RandomScene::onMouseButton(int button, int action)
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

DEFINE_MAIN("RandomScene", RandomScene, nullptr, Trace);