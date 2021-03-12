#include <util/Timer.hpp>

Timer::Timer() : start{}, previous{}, current{} {
    reset();
}

[[nodiscard]] float Timer::get_time() const {
    std::chrono::duration<float> duration = current - start;
    return duration.count();
}

[[nodiscard]] float Timer::get_delta() const {
    std::chrono::duration<float> duration = current - previous;
    return duration.count();
}

void Timer::update() {
    previous = current;
    current = Clock::now();
}

void Timer::reset() {
    start = Clock::now();
    previous = start;
    current = start;
}