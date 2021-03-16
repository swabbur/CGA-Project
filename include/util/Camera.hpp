#pragma once

#include <glm/glm.hpp>

class Window;

class Camera {

    Window const & window;
    glm::vec3 position;
    glm::vec2 rotation;

public:

    Camera(Window & window, glm::vec3 position, glm::vec2 rotation);

    [[nodiscard]] glm::vec3 get_position() const;

    [[nodiscard]] glm::mat4 get_projection_matrix() const;

    [[nodiscard]] glm::mat4 get_view_matrix() const;

    void place(glm::vec3 const & new_position);

    void move(glm::vec3 const & position_delta);

    void turn(glm::vec2 const & rotation);

    void rotate(glm::vec2 const & rotation_delta);
};