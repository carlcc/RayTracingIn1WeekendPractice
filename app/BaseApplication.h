/**
 * Copyright (c) 2018
 * Carl Chen. All rights reserved.
 *
 * This file is part of MSServer.
 * Created by carl on ${DATA}.
 */

#pragma once

#include <cstdint>
#include <string>
#include <glad/glad.h>
#define SDL_MAIN_HANDLED
#include <SDL2/SDL.h>

#include "Log.h"

struct SDL_Window;

class BaseApplication {
public:

    virtual bool initialize();

    virtual void mainLoop();

    virtual void finalize();

    int getWidth() const;

    int getHeight() const;

    void showMouseCursor(bool b);

protected:
    explicit BaseApplication(const std::string& title);

    virtual ~BaseApplication();

    virtual bool initializingBeforeGL() { return true; };

    virtual void tick() { }

    virtual void display(bool autoRedraw = true);

    virtual void onResize(int width, int height);

    int64_t getAppTimeMillis();

    virtual void onKey(int key, int scancode, int action, int mods)
    { /* NOTHING */ }

    virtual void onChar(unsigned int codepoint)
    { /* NOTHING */ }

    virtual void onMouseMove(int x, int y, int dx, int dy);

    virtual void onMouseButton(int button, int action);

    void setIsRunning(bool b);

    bool getIsRunning() const { return mIsRunning; }

private:

    void handleEvents();

    static void char_callback(SDL_Window* window, unsigned int codepoint);

private:
    SDL_Window* mWindow{};
    int64_t mAppStartTime{};
    std::string mTitle;
    bool mIsRunning {true};
};

#define DEFINE_MAIN(title, appClass, logPath, logLevel) \
int main(int argc, char **args)                    \
{                                                    \
    loggerInit(logPath, LogLevel::logLevel);      \
    appClass myApp(title);                       \
    if (myApp.initialize()) {                       \
        myApp.mainLoop();                           \
    }                                               \
    myApp.finalize();                               \
    loggerClean();                                  \
    return 0;                                      \
}

