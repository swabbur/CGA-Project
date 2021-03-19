#pragma once

#include "Material.hpp"
#include "Mesh.hpp"
#include "Shape.hpp"
#include "Texture.hpp"

#include <string>
#include <vector>

class Model {

    std::vector<Material> materials;
    std::vector<Mesh> meshes;

public:

    static Model load(std::string const & path);

    std::vector<Shape> shapes;

    explicit Model(std::vector<Material> && materials,
                   std::vector<Mesh> && meshes,
                   std::vector<Shape> && shapes);

    Model(Model const &) = delete;

    Model(Model && scene) noexcept;

    ~Model();
};
