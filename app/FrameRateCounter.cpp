#include "FrameRateCounter.h"
#include "TimeManager.h"
#include "Log.h"

FrameRateCounter::FrameRateCounter() = default;

FrameRateCounter::~FrameRateCounter() = default;

void FrameRateCounter::reset()
{
    mLastTime = TimeManager::getAppTime();
    mFrameCount = 0;
}

void FrameRateCounter::count()
{
    mFrameCount++;
    int delta = static_cast<int>(TimeManager::getAppTime() - mLastTime);
    if (delta >= mCountPeriod) {
        onFrameTimeout();
        reset();
    }
}

void FrameRateCounter::onFrameTimeout()
{
    float delta = (TimeManager::getAppTime() - mLastTime) / 1000.0f;
    INFO("FrameRate: {}", (mFrameCount / delta));
}
