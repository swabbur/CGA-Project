#pragma once

#include <glm/glm.hpp>

class SpotLight {

public:

    glm::vec3 color;
    glm::vec3 position;
    glm::vec3 direction;
    float angle;
    float intensity;

    SpotLight();

    [[nodiscard]] glm::mat4 get_projection(float near, float far) const;

    [[nodiscard]] glm::mat4 get_view();
};