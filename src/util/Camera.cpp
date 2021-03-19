#include <devices/Window.hpp>
#include <util/Camera.hpp>
#include <glm/gtx/quaternion.hpp>

Camera::Camera(Window & window, glm::vec3 position, glm::vec2 rotation)
    : window(window), position(position), rotation(rotation)
{}

glm::vec3 Camera::get_position() const {
    return position;
}

glm::mat4 Camera::get_projection_matrix() const {
    return glm::perspective(glm::radians(70.0f), window.get_aspect_ratio(), 0.01f, 100.0f);
}

glm::mat4 Camera::get_view_matrix() const {

    glm::mat4 x_rotation_matrix = glm::rotate(glm::mat4(1.0f), rotation.x, glm::vec3(1.0f, 0.0f, 0.0f));
    glm::mat4 y_rotation_matrix = glm::rotate(glm::mat4(1.0f), rotation.y, glm::vec3(0.0f, 1.0f, 0.0f));

    glm::mat4 view_matrix(1.0f);
    view_matrix = view_matrix * x_rotation_matrix;
    view_matrix = view_matrix * y_rotation_matrix;
    view_matrix = glm::translate(view_matrix, -position);
    return view_matrix;
}

void Camera::place(glm::vec3 const & new_position) {
    position = new_position;
}

void Camera::move(glm::vec3 const & position_delta) {
    glm::mat4 y_rotation_matrix = glm::rotate(glm::mat4(1.0f), rotation.y, glm::vec3(0.0f, 1.0f, 0.0f));
    position += glm::vec3(glm::vec4(position_delta, 1.0f) * y_rotation_matrix);
}

void Camera::turn(glm::vec2 const & new_rotation) {
    rotation = new_rotation;
    rotation.x = glm::clamp(rotation.x, -glm::half_pi<float>(), glm::half_pi<float>());
}

void Camera::rotate(glm::vec2 const & rotation_delta) {
    rotation += rotation_delta;
    rotation.x = glm::clamp(rotation.x, -glm::half_pi<float>(), glm::half_pi<float>());
}
