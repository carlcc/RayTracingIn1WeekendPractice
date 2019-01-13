#pragma once

#include <cstdint>
#include <chrono>
/**
 * TimeManager is a class represents
 */
class TimeManager {
public:
    static int64_t getAppStartTime()
    {
        return sAppStartTime;
    }

    static int64_t getAppTime()
    {
        return sAppCurrentTime - sAppStartTime;
    }

    static float getLastFrameDelta()
    {
        return sLastFrameDelta;
    }

    static int64_t getAppCurrentTime()
    {
        return sAppCurrentTime;
    }

    static int64_t steadyTimeMillis()
    {
        using namespace std::chrono;
        return duration_cast<milliseconds>(steady_clock::now().time_since_epoch()).count();
    }

private:
    TimeManager();

    ~TimeManager();

    static void setAppStartTime(int64_t t)
    {
        sAppStartTime = t;
        sAppCurrentTime = 0;
        sAppLastFrameTime = 0;
        sLastFrameDelta = 0.f;
    }

    static void setAppCurrentTime(int64_t t)
    {
        sAppLastFrameTime = sAppCurrentTime;
        sAppCurrentTime = t;
        update();
    }

    static void update()
    {
        sLastFrameDelta = (sAppCurrentTime - sAppLastFrameTime) / 1000.f;
    }

private:
    static int64_t sAppStartTime;
    static int64_t sAppLastFrameTime;
    static int64_t sAppCurrentTime;
    static float sLastFrameDelta;

    friend class BaseApplication;
};

