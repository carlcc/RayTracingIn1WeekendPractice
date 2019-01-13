#pragma once

#include <string>

/**
 * FrameRateCounter is a class represents
 */
class FrameRateCounter {
public:
    FrameRateCounter();

    ~FrameRateCounter();

    void reset();

    void count();

    virtual void onFrameTimeout();

private:
    int mFrameCount {-1};
    int64_t mLastTime;
    int mCountPeriod { 1000 };
};
