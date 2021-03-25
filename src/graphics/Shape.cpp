#include <graphics/Shape.hpp>

Shape::Shape(Material const & material, Mesh const & mesh, AABB const & aabb)
    : material(material), mesh(mesh), aabb(aabb) {}

Mesh const & Shape::get_mesh() const {
    return mesh;
}

AABB const& Shape::get_AABB() const {
    return aabb;
}