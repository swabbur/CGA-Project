#pragma once

#include <glm/glm.hpp>

class DirectionalLight {

public:

    glm::vec3 color;
    glm::vec3 direction;
    float intensity;

    DirectionalLight();

    [[nodiscard]] glm::mat4 get_projection_matrix(float width, float height, float near, float far) const;

    [[nodiscard]] glm::mat4 get_view_matrix(glm::vec3 center) const;
};
