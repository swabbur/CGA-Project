#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <graphics/lights/DirectionalLight.hpp>

DirectionalLight::DirectionalLight() : color(), direction(), intensity() {}

glm::mat4 DirectionalLight::get_projection_matrix(float width, float height, float near, float far) const { // NOLINT(readability-convert-member-functions-to-static)
    return glm::ortho(-width, width, -height, height, near, far);
}

glm::mat4 DirectionalLight::get_view_matrix() const {
    return glm::lookAt(direction, glm::vec3(), glm::vec3(0.0f, 1.0f, 0.0f));
}
