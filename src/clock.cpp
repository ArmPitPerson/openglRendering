#include "clock.h"

Clock::Clock() : mStarted(now())
{
}

const Clock::TimeUnit Clock::timeSinceStart() const
{
    return TimeUnit{ now() - mStarted };
}

const Clock::TimeUnit Clock::restart()
{
    auto out = timeSinceStart();
    mStarted = now();
    return out;
}

const Clock::TimePoint Clock::now() const
{
    return std::chrono::steady_clock::now();
}
