#pragma once

#include <glm/glm.hpp>
#include <variant>

class Texture;

class Material {

public:

    std::variant<std::monostate, glm::vec3, Texture const *> ambient;
    std::variant<std::monostate, glm::vec3, Texture const *> diffuse;
    std::variant<std::monostate, glm::vec3, Texture const *> specular;
    std::variant<std::monostate, float, Texture const *> roughness;
    std::variant<std::monostate, Texture const *> normal;

    Material();
};
