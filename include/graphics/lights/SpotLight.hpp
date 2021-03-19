#pragma once

#include <glm/glm.hpp>

class SpotLight {

public:

    glm::vec3 color;
    glm::vec3 position;
    glm::vec3 direction;
    float intensity;
    float angle;

    SpotLight();
};