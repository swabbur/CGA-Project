#pragma once

#include <glm/glm.hpp>

class DirectionalLight {

public:

    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;
    glm::vec3 direction;

    DirectionalLight();
};