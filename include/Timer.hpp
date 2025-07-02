#pragma once

#include <string>

class Timer {
public:
    void start();
    void stop();
    std::string get() const;
    void print() const;

    static std::string now();

private:
    using Clock     = std::chrono::high_resolution_clock;
    using TimePoint = Clock::time_point;

    bool running = false;
    TimePoint startTime;
    TimePoint endTime;
};