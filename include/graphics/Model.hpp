#pragma once

#include "Material.hpp"
#include "Mesh.hpp"
#include "Shape.hpp"

#include <string>
#include <vector>

class Model {

public:

    static Model load(std::string const & path);

    std::vector<Material> materials;
    std::vector<Mesh> meshes;
    std::vector<Shape> shapes;

    Model(std::vector<Material> && materials, std::vector<Mesh> && meshes, std::vector<Shape> && shapes);

    Model(Model const &) = delete;

    Model(Model && scene) noexcept;

    ~Model();
};
