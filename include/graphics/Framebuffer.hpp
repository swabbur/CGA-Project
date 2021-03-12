#pragma once

class Framebuffer {

    unsigned int handle;

    explicit Framebuffer(unsigned int handle);

public:

    static Framebuffer get_default();

    Framebuffer(Framebuffer const &) = delete;

    Framebuffer(Framebuffer && framebuffer) noexcept;

    ~Framebuffer();

    void bind() const;
};