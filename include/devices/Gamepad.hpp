#pragma once

#include <map>
#include <set>

class Gamepad {

    friend class DeviceManager;

    bool connected;
    std::map<unsigned char, float> axes;
    std::set<unsigned char> pressed;
    std::set<unsigned char> released;
    std::set<unsigned char> down;

    Gamepad();

public:

    Gamepad(Gamepad const &) = delete;

    Gamepad(Gamepad &&) = delete;

    ~Gamepad();

    [[nodiscard]] bool is_connected() const;

    [[nodiscard]] float get_axis(unsigned int axis) const;

    [[nodiscard]] bool is_pressed(unsigned char button) const;

    [[nodiscard]] bool is_released(unsigned char button) const;

    [[nodiscard]] bool is_down(unsigned char button) const;

    void poll();
};
