#include <devices/Window.hpp>
#include <util/Camera.hpp>
#include <glm/gtx/quaternion.hpp>

float get_angle(glm::vec2 a, glm::vec2 b) {
    glm::vec2 na = glm::normalize(a);
    glm::vec2 nb = glm::normalize(b);
    float dot = glm::dot(na, nb);
    float determinant = (na.x * nb.y) - (na.y * nb.x);
    return glm::atan(determinant, dot);
}

Camera::Camera() : position(), rotation(), aspect_ratio(1.0f) {}

glm::vec3 Camera::get_position() const {
    return position;
}

glm::mat4 Camera::get_projection_matrix() const {
    return glm::perspective(glm::radians(70.0f), aspect_ratio, 0.01f, 100.0f);
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

void Camera::set_aspect_ratio(float new_aspect_ratio) {
    aspect_ratio = new_aspect_ratio;
}

void Camera::place(glm::vec3 const & new_position) {
    position = new_position;
}

void Camera::move(glm::vec3 const & position_delta) {
    glm::mat4 y_rotation_matrix = glm::rotate(glm::mat4(1.0f), rotation.y, glm::vec3(0.0f, 1.0f, 0.0f));
    position += glm::vec3(glm::vec4(position_delta, 1.0f) * y_rotation_matrix);
}

void Camera::set_forward(glm::vec3 const & direction) {
    glm::vec3 forward(0.0f, 0.0f, -1.0f);
    glm::vec3 opposite = -direction;
    rotation.x = get_angle(glm::vec2(forward.y, forward.z), glm::vec2(opposite.y, opposite.z));
    rotation.y = get_angle(glm::vec2(forward.x, forward.z), glm::vec2(opposite.x, opposite.z));
}

void Camera::turn(glm::vec2 const & new_rotation) {
    rotation = new_rotation;
    rotation.x = glm::clamp(rotation.x, -glm::half_pi<float>(), glm::half_pi<float>());
}

void Camera::rotate(glm::vec2 const & rotation_delta) {
    rotation += rotation_delta;
    rotation.x = glm::clamp(rotation.x, -glm::half_pi<float>(), glm::half_pi<float>());
}