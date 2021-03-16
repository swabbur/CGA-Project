#pragma once

#include <glm/glm.hpp>

class SpotLight {

public:

    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;
    glm::vec3 direction;
    glm::vec3 position;
    struct {
        float inner;
        float outer;
    } angles;

    SpotLight();
};