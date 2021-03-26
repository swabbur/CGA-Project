#pragma once

#include "Material.hpp"
#include "Mesh.hpp"
#include <physics/BoundingBox.hpp>

#include <glm/glm.hpp>

class Shape {

public:

    Material const & material;
    Mesh const & mesh;
    BoundingBox const bounding_box;

    Shape(Material const & material, Mesh const & mesh, BoundingBox const & bounding_box);

    [[nodiscard]] Mesh const & get_mesh() const;

    [[nodiscard]] BoundingBox const& get_bounding_box() const;
};
