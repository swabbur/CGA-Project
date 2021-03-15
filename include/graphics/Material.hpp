#pragma once

#include "Texture.hpp"

#include <glm/glm.hpp>
#include <variant>

class Material {

public:

    std::variant<std::monostate, glm::vec3, Texture> ambient;
    std::variant<std::monostate, glm::vec3, Texture> diffuse;
    std::variant<std::monostate, glm::vec3, Texture> specular;
    std::variant<std::monostate, float, Texture> roughness;
    std::variant<std::monostate, Texture> normal;

    Material();
};
