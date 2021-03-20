#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <graphics/lights/SpotLight.hpp>

SpotLight::SpotLight() : color(), direction(), position(), angle(), intensity() {}

glm::mat4 SpotLight::get_projection(float near, float far) const {
    return glm::perspective(angle, 1.0f, near, far);
}

glm::mat4 SpotLight::get_view() const {
    glm::mat4 view_matrix;
    view_matrix = glm::lookAt(-direction, glm::vec3(), glm::vec3(1.0f, 0.0f, 0.0f));
    view_matrix = glm::translate(view_matrix, -position);
    return view_matrix;
}
