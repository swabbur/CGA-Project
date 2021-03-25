#pragma once

#include "Mesh.hpp"
#include "Texture.hpp"

#include <glm/glm.hpp>
#include <string>

class Icon {

public:

    static Icon load(std::string const & path);

    Texture texture;
    Mesh mesh;

    float scale;
    glm::vec2 position;
    bool visible;

    Icon(Mesh && mesh, Texture && texture, float scale, glm::vec2 position, bool visible);

    Icon(Icon const &) = delete;

    Icon(Icon && icon) noexcept;

    ~Icon();
};
