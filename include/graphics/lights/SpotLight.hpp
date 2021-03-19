#pragma once

#include <glm/glm.hpp>

class SpotLight {

public:

    glm::vec3 color;
    glm::vec3 direction;
    glm::vec3 position;
    float angle;

    SpotLight();
};