#pragma once

#include "Material.hpp"
#include "Mesh.hpp"
#include "Shape.hpp"
#include "Texture.hpp"

#include <string>
#include <vector>

class Scene {

    std::vector<Material> materials;
    std::vector<Mesh> meshes;

public:

    static Scene load(std::string const & path);

    std::vector<Shape> shapes;

    explicit Scene(std::vector<Material> && materials,
                   std::vector<Mesh> && meshes,
                   std::vector<Shape> && shapes);

    Scene(Scene const &) = delete;

    Scene(Scene && scene) noexcept;

    ~Scene();
};
