#pragma once

#include <glm/glm.hpp>

class DirectionalLight {

public:

    glm::vec3 color;
    glm::vec3 direction;
    float intensity;

    DirectionalLight();

    [[nodiscard]] glm::mat4 get_projection(float width, float height, float near, float far) const;

    [[nodiscard]] glm::mat4 get_view() const;
};
