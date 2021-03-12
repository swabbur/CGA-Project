#pragma once

#include <mutex>
#include <set>

class Keyboard {

    friend class DeviceManager;

    struct State {
        std::set<int> pressed;
        std::set<int> released;
        std::set<int> down;
    };

    void * handle;

    std::mutex mutex;
    State latest;
    State polled;

    Keyboard();

    void set_handle(void * new_handle);

    void press(int key);

    void release(int key);

public:

    Keyboard(Keyboard const &) = delete;

    Keyboard(Keyboard &&) = delete;

    ~Keyboard();

    [[nodiscard]] bool is_pressed(int key) const;

    [[nodiscard]] bool is_released(int key) const;

    [[nodiscard]] bool is_down(int key) const;

    void poll();
};