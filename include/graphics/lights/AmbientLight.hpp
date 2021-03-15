#pragma once

#include <glm/glm.hpp>

class AmbientLight {

public:

    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;

    AmbientLight();
};