#pragma once

#include <glm/glm.hpp>

class Window;

class Camera {

    float aspect_ratio;
    glm::vec3 center;
    glm::vec3 inverse_direction;
    float scale;

public:

    Camera(glm::vec3 center, glm::vec3 inverse_direction);

    [[nodiscard]] glm::mat4 get_projection_matrix() const;

    [[nodiscard]] glm::mat4 get_view_matrix() const;

    [[nodiscard]] glm::vec3 get_position() const;

    [[nodiscard]] glm::vec3 get_direction() const;

    void set_aspect_ratio(float new_aspect_ratio);

    void focus(glm::vec3 position);

    void zoom(float delta);
};