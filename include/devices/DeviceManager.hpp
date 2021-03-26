#pragma once

#include "Gamepad.hpp"
#include "Keyboard.hpp"
#include "Mouse.hpp"
#include "Window.hpp"

#include <atomic>
#include <thread>

class DeviceManager {

    void * handle;

    Keyboard keyboard;
    Mouse mouse;
    Window window;
    Gamepad gamepad;

    std::atomic_bool running;
    std::thread thread;

public:

    DeviceManager();

    DeviceManager(DeviceManager const &) = delete;

    DeviceManager(DeviceManager &) = delete;

    ~DeviceManager();

    [[nodiscard]] Gamepad & get_gamepad();

    [[nodiscard]] Keyboard & get_keyboard();

    [[nodiscard]] Mouse & get_mouse();

    [[nodiscard]] Window & get_window();
};