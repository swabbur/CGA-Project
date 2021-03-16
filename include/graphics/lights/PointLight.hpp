#pragma once

#include <glm/glm.hpp>

class PointLight {

public:

    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;
    glm::vec3 position;

    PointLight();
};