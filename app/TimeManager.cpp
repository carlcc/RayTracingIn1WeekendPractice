#include "TimeManager.h"


int64_t TimeManager::sAppStartTime = 0;
int64_t TimeManager::sAppLastFrameTime = 0;
int64_t TimeManager::sAppCurrentTime = 0;
float TimeManager::sLastFrameDelta = 0.f;

TimeManager::TimeManager() = default;

TimeManager::~TimeManager() = default;
