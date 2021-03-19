#pragma once

#include <glm/glm.hpp>

class DirectionalLight {

public:

    glm::vec3 color;
    glm::vec3 direction;

    DirectionalLight();
};