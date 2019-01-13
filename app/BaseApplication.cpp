/**
 * Copyright (c) 2018
 * Carl Chen. All rights reserved.
 *
 * This file is part of MSServer.
 * Created by carl on ${DATA}.
 */

#include "BaseApplication.h"
#include <glad/glad.h>
#include <SDL2/SDL_opengl.h>
#include "TimeManager.h"

BaseApplication::BaseApplication(const std::string& title): mWindow(nullptr), mTitle(title)
{
    TimeManager::setAppStartTime(TimeManager::steadyTimeMillis());
}

BaseApplication::~BaseApplication()
{
    if (mWindow != nullptr) {
        SDL_DestroyWindow(mWindow);
        mWindow = nullptr;
    }
}

bool BaseApplication::initialize()
{
    int mav, miv;
    SDL_Init(SDL_INIT_VIDEO);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

    uint32_t WindowFlags = SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE;
    mWindow = SDL_CreateWindow(mTitle.c_str(), 100, 100, 640, 480, WindowFlags);
    if (mWindow == nullptr) {
        return false;
    }

    if (!initializingBeforeGL()) {
        return false;
    }

    SDL_SetWindowData(mWindow, "this", this);
    SDL_GLContext context = SDL_GL_CreateContext(mWindow);
    SDL_GL_MakeCurrent(mWindow, context);

    gladLoadGL();
    SDL_SetWindowSize(mWindow, 640, 480);

    const GLubyte* name = glGetString(GL_VENDOR); //返回负责当前OpenGL实现厂商的名字
    const GLubyte* biaoshifu = glGetString(GL_RENDERER); //返回一个渲染器标识符，通常是个硬件平台
    const GLubyte* OpenGLVersion =glGetString(GL_VERSION); //返回当前OpenGL实现的版本号
    const GLubyte* Extensions  =glGetString(GL_EXTENSIONS);
    INFO("OpenGL实现厂商的名字：{}", (const char*)name);
    INFO("渲染器标识符：{}", (const char*)biaoshifu);
    INFO("OpenGL实现的版本号：{}",(const char*)OpenGLVersion );
    INFO("OpenGL支持的扩展：{}",Extensions == nullptr ? "null" : (const char*)Extensions );

    SDL_GL_SetSwapInterval(1);

    return context != nullptr;
}

void BaseApplication::display(bool autoRedraw)
{
    SDL_GL_SwapWindow(mWindow);
}

void BaseApplication::mainLoop()
{
    do {
        TimeManager::setAppCurrentTime(TimeManager::steadyTimeMillis());
        tick();
        display();
        handleEvents();
    } while (getIsRunning());
}

void BaseApplication::finalize()
{

}

void BaseApplication::onResize(int width, int height)
{
    glViewport(0, 0, width, height);
}

int64_t BaseApplication::getAppTimeMillis()
{
    return TimeManager::getAppTime();
}

void BaseApplication::setIsRunning(bool b)
{
    mIsRunning = b;
}

void BaseApplication::handleEvents()
{
    SDL_Event event;
    while (SDL_PollEvent(&event))
    {
        if (event.type == SDL_KEYDOWN || event.type == SDL_KEYUP)
        {
            this->onKey(event.key.keysym.sym, event.key.keysym.scancode, event.type, event.key.keysym.mod);
        } else if (event.type == SDL_WINDOWEVENT) {
            if (event.window.event == SDL_WINDOWEVENT_SIZE_CHANGED/* || event.window.event == SDL_WINDOWEVENT_RESIZED*/) {
                this->onResize(event.window.data1, event.window.data2);
            }
        } else if (event.type == SDL_QUIT) {
            mIsRunning = false;
        } else if (event.type == SDL_MOUSEMOTION) {
            this->onMouseMove(event.motion.x, event.motion.y, event.motion.xrel, event.motion.yrel);
        } else if (event.type == SDL_MOUSEBUTTONDOWN || event.type == SDL_MOUSEBUTTONUP) {
            this->onMouseButton(event.button.button, event.type);
        }
    }
}

void BaseApplication::char_callback(SDL_Window* window, unsigned int codepoint)
{
    auto* app = static_cast<BaseApplication*>(SDL_GetWindowData(window, "this"));
    app->onChar(codepoint);
}

int BaseApplication::getWidth() const
{
    int w;
    SDL_GetWindowSize(mWindow, &w, nullptr);
    return w;
}

int BaseApplication::getHeight() const
{
    int h;
    SDL_GetWindowSize(mWindow, nullptr, &h);
    return h;
}

void BaseApplication::onMouseMove(int x, int y, int dx, int dy)
{

}

void BaseApplication::onMouseButton(int button, int action)
{

}

void BaseApplication::showMouseCursor(bool b)
{
    SDL_ShowCursor(b ? SDL_ENABLE : SDL_DISABLE);
}

