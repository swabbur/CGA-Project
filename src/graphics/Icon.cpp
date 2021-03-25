#include <graphics/Icon.hpp>

Icon Icon::load(std::string const & path) {
    Mesh mesh = Mesh::quad();
    Texture texture = Texture::load(path);
    return Icon(std::move(mesh), std::move(texture), 1.0f, glm::vec2(), true);
}

Icon::Icon(Mesh && mesh, Texture && texture, float scale, glm::vec2 position, bool visible) :
    mesh(std::move(mesh)), texture(std::move(texture)), scale(scale), position(position), visible(visible) {}

Icon::Icon(Icon && icon) noexcept :
    mesh(std::move(icon.mesh)),
    texture(std::move(icon.texture)),
    scale(icon.scale),
    position(icon.position),
    visible(icon.visible) {}

Icon::~Icon() = default;
