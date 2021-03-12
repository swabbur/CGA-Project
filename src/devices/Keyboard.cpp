#include <devices/Keyboard.hpp>

Keyboard::Keyboard() : handle(nullptr), mutex(), latest{}, polled(latest) {}

Keyboard::~Keyboard() = default;

void Keyboard::set_handle(void * new_handle) {
    handle = new_handle;
}

[[nodiscard]] bool Keyboard::is_pressed(int key) const {
    return polled.pressed.find(key) != polled.pressed.end();
}

[[nodiscard]] bool Keyboard::is_released(int key) const {
    return polled.released.find(key) != polled.released.end();
}

[[nodiscard]] bool Keyboard::is_down(int key) const {
    return polled.down.find(key) != polled.down.end();
}

void Keyboard::press(int key) {
    std::scoped_lock<std::mutex> lock(mutex);
    latest.pressed.insert(key);
    latest.down.insert(key);
}

void Keyboard::release(int key) {
    std::scoped_lock<std::mutex> lock(mutex);
    latest.released.insert(key);
    latest.down.erase(key);
}

void Keyboard::poll() {
    std::scoped_lock<std::mutex> lock(mutex);
    polled = latest;
    latest.pressed.clear();
    latest.released.clear();
}