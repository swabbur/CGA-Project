#include <graphics/lights/SpotLight.hpp>

SpotLight::SpotLight() : color(), direction(), position(), angle(), intensity() {}

glm::mat4 SpotLight::get_projection(float near, float far) const {
    return glm::mat4(1.0f);
}

glm::mat4 SpotLight::get_view() {
    return glm::mat4(1.0f);
}