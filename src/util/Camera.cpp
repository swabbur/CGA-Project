#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <util/Camera.hpp>

Camera::Camera(glm::vec3 center, glm::vec3 offset) : aspect_ratio(1.0f), center(center), offset(offset) {}

glm::mat4 Camera::get_projection_matrix() const {
    return glm::perspective(glm::radians(70.0f), aspect_ratio, 0.01f, 100.0f);
}

glm::mat4 Camera::get_view_matrix() const {
    return glm::lookAt(center + offset, center, glm::vec3(0.0f, 1.0f, 0.0f));
}

glm::vec3 Camera::get_position() const {
    return center + offset;
}

glm::vec3 Camera::get_direction() const {
    return glm::normalize(-offset);
}

void Camera::set_aspect_ratio(float new_aspect_ratio) {
    aspect_ratio = new_aspect_ratio;
}

void Camera::focus(glm::vec3 position) {
    center = position;
}
