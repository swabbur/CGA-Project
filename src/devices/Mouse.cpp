#include <devices/Mouse.hpp>

Mouse::Mouse() : handle(nullptr), mutex(), latest{}, polled(latest), dx(0.0f), dy(0.0f), moved(false) {}

Mouse::~Mouse() = default;

void Mouse::set_handle(void * new_handle) {
    handle = new_handle;
}

bool Mouse::is_pressed(int button) const {
    return polled.pressed.find(button) != polled.pressed.end();
}

bool Mouse::is_released(int button) const {
    return polled.released.find(button) != polled.released.end();
}

bool Mouse::is_down(int button) const {
    return polled.down.find(button) != polled.down.end();
}

float Mouse::get_x() const {
    return polled.x;
}

float Mouse::get_y() const {
    return polled.y;
}

float Mouse::get_dx() const {
    return dx;
}

float Mouse::get_dy() const {
    return dy;
}

bool Mouse::is_moved() const {
    return moved;
}

float Mouse::get_scroll_x() const {
    return polled.scroll_x;
}

float Mouse::get_scroll_y() const {
    return polled.scroll_y;
}

bool Mouse::is_scrolled() const {
    return scrolled;
}

void Mouse::press(int button) {
    std::scoped_lock<std::mutex> lock(mutex);
    latest.pressed.insert(button);
    latest.down.insert(button);
}

void Mouse::release(int button) {
    std::scoped_lock<std::mutex> lock(mutex);
    latest.released.insert(button);
    latest.down.erase(button);
}

void Mouse::move(float x, float y) {
    std::scoped_lock<std::mutex> lock(mutex);
    latest.x = x;
    latest.y = y;
}

void Mouse::scroll(float x, float y) {
    std::scoped_lock<std::mutex> lock(mutex);
    latest.scroll_x += x;
    latest.scroll_y += y;
}

void Mouse::poll() {
    std::scoped_lock<std::mutex> lock(mutex);
    dx = latest.x - polled.x;
    dy = latest.y - polled.y;
    moved = (polled.x != latest.x) || (polled.y != latest.y);
    scrolled = (polled.scroll_x != latest.scroll_x) || (polled.scroll_y != latest.scroll_y);
    polled = latest;
    latest.scroll_x = 0.0f;
    latest.scroll_y = 0.0f;
}
