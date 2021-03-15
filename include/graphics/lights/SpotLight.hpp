#pragma once

#include "Attenuation.hpp"

#include <glm/glm.hpp>

class SpotLight {

public:

    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;
    glm::vec3 direction;
    glm::vec3 position;
    Attenuation attenuation;
    struct {
        float inner;
        float outer;
    } angles;

    SpotLight();
};