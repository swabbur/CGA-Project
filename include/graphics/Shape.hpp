#pragma once

#include "Material.hpp"
#include "Mesh.hpp"

#include <glm/glm.hpp>

class Shape {

public:

    Material material;
    Mesh mesh;

    Shape(Material && material, Mesh && mesh);

    Shape(Shape const &) = delete;

    Shape(Shape && shape) noexcept;

    ~Shape();
};
