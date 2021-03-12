#pragma once

#include <mutex>
#include <set>

class Mouse {

    friend class DeviceManager;

    struct State {
        std::set<int> pressed;
        std::set<int> released;
        std::set<int> down;
        float x;
        float y;
    };

    void * handle;

    std::mutex mutex;
    State latest;
    State polled;
    float dx;
    float dy;
    bool moved;

    Mouse();

    void set_handle(void * new_handle);

    void press(int button);

    void release(int button);

    void move(float x, float y);

public:


    Mouse(Mouse const &) = delete;

    Mouse(Mouse &&) = delete;

    ~Mouse();

    [[nodiscard]] bool is_pressed(int button) const;

    [[nodiscard]] bool is_released(int button) const;

    [[nodiscard]] bool is_down(int button) const;

    [[nodiscard]] float get_x() const;

    [[nodiscard]] float get_y() const;

    [[nodiscard]] float get_dx() const;

    [[nodiscard]] float get_dy() const;

    [[nodiscard]] bool is_moved() const;

    void poll();
};