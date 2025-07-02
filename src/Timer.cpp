#include "Timer.hpp"

#include <chrono>
#include <ctime>
#include <iomanip>
#include <iostream>
#include <sstream>

void Timer::start() {
    startTime = Clock::now();
    running = true;
}

void Timer::stop() {
    if (!running) {
        std::cerr << "[Timer] Error: stop() called before start()." << std::endl;
        return;
    }
    endTime = Clock::now();
    running = false;
}

std::string Timer::get() const {
    if (running) {
        std::cerr << "[Timer] Warning: Timer is still running." << std::endl;
        return "";
    }

    auto duration = std::chrono::duration_cast<std::chrono::seconds>(endTime - startTime);
    int totalSec = static_cast<int>(duration.count());

    int hours   = totalSec / 3600;
    int minutes = (totalSec % 3600) / 60;
    int seconds = totalSec % 60;

    std::ostringstream oss;
    oss << std::setw(2) << std::setfill('0') << hours << ":"
        << std::setw(2) << std::setfill('0') << minutes << ":"
        << std::setw(2) << std::setfill('0') << seconds;

    return oss.str();
}

void Timer::print() const {
    if (running) {
        std::cerr << "[Timer] Warning: Timer is still running." << std::endl;
        return;
    }
    std::cout << "Time : " << get() << std::endl;
}

std::string Timer::now() {
    auto now = std::chrono::system_clock::now();
    std::time_t timeT = std::chrono::system_clock::to_time_t(now);
    std::tm localTime = *std::localtime(&timeT);

    std::ostringstream oss;
    oss << std::put_time(&localTime, "%Y-%m-%d %H:%M:%S");
    return oss.str();
}