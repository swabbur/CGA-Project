#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <util/Camera.hpp>

Camera::Camera(glm::vec3 center, glm::vec3 inverse_direction) :
    aspect_ratio(1.0f), center(center), inverse_direction(glm::normalize(inverse_direction)), scale(4.0f) {}

glm::mat4 Camera::get_projection_matrix() const {
    return glm::perspective(glm::radians(70.0f), aspect_ratio, 0.01f, 100.0f);
}

glm::mat4 Camera::get_view_matrix() const {
    return glm::lookAt(center + inverse_direction * scale, center, glm::vec3(0.0f, 1.0f, 0.0f));
}

glm::vec3 Camera::get_position() const {
    return center + inverse_direction * scale;
}

glm::vec3 Camera::get_focus_point() const {
    return center;
}

void Camera::set_aspect_ratio(float new_aspect_ratio) {
    aspect_ratio = new_aspect_ratio;
}

void Camera::focus(glm::vec3 position) {
    center = position;
}

void Camera::zoom(float delta) {
    scale = glm::clamp(scale + delta, 1.0f, 4.0f);
}
