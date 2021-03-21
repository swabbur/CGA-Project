#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <graphics/lights/SpotLight.hpp>

SpotLight::SpotLight() : color(), direction(), position(), angle(), intensity() {}

glm::mat4 SpotLight::get_projection(float near, float far) const {

    // TODO: verify that angle is correct here, may need to be multiplied by 2.

    return glm::perspective(angle, 1.0f, near, far);
}

glm::mat4 SpotLight::get_view() const {

    // TODO: Properly compute up vector.

    glm::mat4 rotation_matrix = glm::lookAt(position, position + direction, glm::vec3(1.0f, 0.0f, 0.0f));
    glm::mat4 view_matrix(1.0f);
    view_matrix = view_matrix * rotation_matrix;
    view_matrix = glm::translate(view_matrix, -position);
    return view_matrix;
}
