#pragma once

#include "Material.hpp"
#include "Mesh.hpp"

#include <glm/glm.hpp>

class Shape {

public:

    Material const & material;
    Mesh const & mesh;

    Shape(Material const & material, Mesh const & mesh);
};
