#pragma once

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

    std::atomic_bool running;
    std::thread thread;

public:

    DeviceManager();

    DeviceManager(DeviceManager const &) = delete;

    DeviceManager(DeviceManager &) = delete;

    ~DeviceManager();

    [[nodiscard]] Keyboard & get_keyboard();

    [[nodiscard]] Mouse & get_mouse();

    [[nodiscard]] Window & get_window();
};