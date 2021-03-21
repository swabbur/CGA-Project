#include <graphics/ShadowMap.hpp>

ShadowMap ShadowMap::create(int size) {
    Texture texture = Texture::create(Texture::Type::DEPTH, size, size);
    Framebuffer framebuffer = Framebuffer::create({ texture });
    return ShadowMap(size, std::move(texture), std::move(framebuffer));
}

ShadowMap::ShadowMap(int size, Texture && texture, Framebuffer && framebuffer)
    : size(size), texture(std::move(texture)), framebuffer(std::move(framebuffer)) {}

ShadowMap::ShadowMap(ShadowMap && shadow_map) noexcept
    : size(shadow_map.size), texture(std::move(shadow_map.texture)), framebuffer(std::move(shadow_map.framebuffer)) {}

ShadowMap::~ShadowMap() = default;
