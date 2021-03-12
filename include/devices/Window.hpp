#pragma once

#include <mutex>

class Window {

    friend class DeviceManager;

    struct State {
        bool closed;
        int width;
        int height;
    };

    void * handle;

    std::mutex mutex;
    State latest;
    State polled;
    bool resized;

    Window();

    void set_handle(void * new_handle);

    void resize(int width, int height);

    void close();

public:

    Window(Window const &) = delete;

    Window(Window &&) = delete;

    ~Window();

    [[nodiscard]] bool is_closed() const;

    [[nodiscard]] int get_width() const;

    [[nodiscard]] int get_height() const;

    [[nodiscard]] float get_aspect_ratio() const;

    [[nodiscard]] bool is_resized() const;

    void make_context_current();

    void swap_buffers();

    void poll();
};
