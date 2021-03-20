#pragma once

#include <glm/glm.hpp>

class PointLight {

public:

    glm::vec3 color;
    glm::vec3 position;
    float intensity;

    PointLight();
};