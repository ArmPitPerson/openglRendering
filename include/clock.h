/// OpenGL - by Carl Findahl - 2018

/* 
 * A clock for measuring time, intended to use
 * for delta time computation and frame times, but
 * is also good for general time tracking. Do note this
 * is not a countdown timer, but more of a stopwatch.
 */

#ifndef CLOCK_H
#define CLOCK_H

#include <chrono>

class Clock
{
public:
    // Float seconds, the time unit the clock uses
    using TimeUnit = std::chrono::duration<float>;
    using TimePoint = std::chrono::steady_clock::time_point;

    Clock();

    // Get the time since start
    const TimeUnit timeSinceStart() const;

    // Restart clock and return the time it had before resetting
    const TimeUnit restart();

private:
    // Return the current time (implementation helper for restart/timeSinceStart)
    const TimePoint now() const;

    // When the clock started
    TimePoint mStarted;

};

#endif // CLOCK_H
