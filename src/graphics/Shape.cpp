#include <graphics/Shape.hpp>

Shape::Shape(Material const & material, Mesh const & mesh, BoundingBox const & bounding_box)
    : material(material), mesh(mesh), bounding_box(bounding_box) {}

Mesh const & Shape::get_mesh() const {
    return mesh;
}

BoundingBox const& Shape::get_bounding_box() const {
    return bounding_box;
}