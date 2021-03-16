#pragma once

#include <functional>
#include <vector>

class Texture;

class Framebuffer {

    unsigned int handle;

    explicit Framebuffer(unsigned int handle);

public:

    static Framebuffer get_default();

    static Framebuffer create(std::vector<std::reference_wrapper<Texture>> const & textures);

    Framebuffer(Framebuffer const &) = delete;

    Framebuffer(Framebuffer && framebuffer) noexcept;

    ~Framebuffer();

    void bind() const;
};