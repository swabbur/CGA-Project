#pragma once

#include "Texture.hpp"
#include "Framebuffer.hpp"

class ShadowMap {

    ShadowMap(int size, Texture && texture, Framebuffer && framebuffer);

public:

    static ShadowMap create(int size);

    int size;
    Texture texture;
    Framebuffer framebuffer;

    ShadowMap(ShadowMap const &) = delete;

    ShadowMap(ShadowMap && shadow_map) noexcept;

    ~ShadowMap();
};