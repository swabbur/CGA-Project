#pragma once

#include <glm/glm.hpp>
#include <variant>

class Texture;

class Material {

    std::variant<std::monostate, glm::vec4, Texture const *> diffuse;
    std::variant<std::monostate, glm::vec4, Texture const *> specular;
    std::variant<std::monostate, float, Texture const *> roughness;
    std::variant<std::monostate, Texture const *> normal;

public:

    Material();
};
