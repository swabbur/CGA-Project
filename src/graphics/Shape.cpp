#include <graphics/Shape.hpp>

Shape::Shape(Material const & material, Mesh const & mesh)
    : material(material), mesh(mesh) {}

Mesh const & Shape::get_mesh() const {
    return mesh;
}