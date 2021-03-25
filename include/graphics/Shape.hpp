#pragma once

#include "Material.hpp"
#include "Mesh.hpp"
#include <physics/AABB.hpp>

#include <glm/glm.hpp>

class Shape {

public:

    Material const & material;
    Mesh const & mesh;
    AABB const aabb;

    Shape(Material const & material, Mesh const & mesh, AABB const & aabb);

    Mesh const & get_mesh() const;

    AABB const& get_AABB() const;
};
