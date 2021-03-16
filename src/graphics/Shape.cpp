#include <graphics/Shape.hpp>

Shape::Shape(Material const & material, Mesh const & mesh)
    : material(material), mesh(mesh) {}
