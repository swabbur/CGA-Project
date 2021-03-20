#pragma once

#include <glm/glm.hpp>

class AmbientLight {

public:

    glm::vec3 color;
    float intensity;

    AmbientLight();
};