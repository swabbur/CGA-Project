#pragma once

#include "Material.hpp"
#include "Mesh.hpp"
#include "Shape.hpp"
#include "Texture.hpp"

#include <string>
#include <vector>

class Scene {

    std::vector<Texture> textures;
    std::vector<Material> materials;
    std::vector<Mesh> meshes;
    std::vector<Shape> shapes;

public:

    static Scene load(std::string const & path);

    Scene(std::vector<Texture> && textures,
          std::vector<Material> && materials,
          std::vector<Mesh> && meshes,
          std::vector<Shape> && shapes);

    Scene(Scene const &) = delete;

    Scene(Scene && scene) noexcept;

    ~Scene();

    void draw();
};
