#pragma once

#include <chrono>

class Timer {

    using Clock = std::chrono::steady_clock;

    std::chrono::time_point<Clock> start;
    std::chrono::time_point<Clock> previous;
    std::chrono::time_point<Clock> current;

public:

    Timer();

    [[nodiscard]] float get_time() const;

    [[nodiscard]] float get_delta() const;

    void update();

    void reset();
};