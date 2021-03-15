#include <graphics/Shape.hpp>

Shape::Shape(Material && material, Mesh && mesh)
    : material(std::move(material)), mesh(std::move(mesh)) {}

Shape::Shape(Shape && shape) noexcept : material(std::move(shape.material)), mesh(std::move(shape.mesh)) {}

Shape::~Shape() = default;