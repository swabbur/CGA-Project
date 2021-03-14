#include <graphics/Shape.hpp>

Shape::Shape(Material const & material, Mesh const & mesh, glm::mat4 transformation) :
    material(material), mesh(mesh), transformation(transformation) {}
