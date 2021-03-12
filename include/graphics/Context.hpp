#pragma once

class Window;

class Context {

public:

    explicit Context(Window & window);

    Context(Context const &) = delete;

    Context(Context &&) = delete;

    ~Context();

    void set_view_port(unsigned int x, unsigned int y, unsigned int width, unsigned int height);

    void set_clear_color(float red, float green, float blue);

    void set_clear_depth(float depth);

    void clear();

    void set_depth_test(bool depth_test);

    void set_cull_face(bool cull_face);

    void set_alpha_blending(bool alpha_blending);
};