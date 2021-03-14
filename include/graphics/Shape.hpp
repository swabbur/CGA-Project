#pragma once

#include <glm/glm.hpp>

class Material;
class Mesh;

class Shape {

    Material const & material;
    Mesh const & mesh;
    glm::mat4 transformation;

public:

    Shape(Material const & material, Mesh const & mesh, glm::mat4 transformation);
};
