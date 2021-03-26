#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <graphics/lights/SpotLight.hpp>

SpotLight::SpotLight() : color(), direction(), position(), angle(), intensity() {}

glm::mat4 SpotLight::get_projection_matrix(float near, float far) const {
    return glm::perspective(2.0f * angle, 1.0f, near, far);
}

glm::mat4 SpotLight::get_view_matrix() const {
    return glm::lookAt(position, position + direction, glm::vec3(0.0f, 1.0f, 0.0f));
}
