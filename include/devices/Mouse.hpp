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
        float scroll_x;
        float scroll_y;
    };

    void * handle;

    std::mutex mutex;
    State latest;
    State polled;
    float dx;
    float dy;
    bool moved;
    bool scrolled;

    Mouse();

    void set_handle(void * new_handle);

    void press(int button);

    void release(int button);

    void move(float x, float y);

    void scroll(float x, float y);

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

    [[nodiscard]] float get_scroll_x() const;

    [[nodiscard]] float get_scroll_y() const;

    [[nodiscard]] bool is_scrolled() const;

    void poll();
};