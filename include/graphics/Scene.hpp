#pragma once

#include "Material.hpp"
#include "Mesh.hpp"
#include "Shape.hpp"
#include "Texture.hpp"

#include <string>
#include <vector>

class Scene {

public:

    std::vector<Shape> shapes;

    static Scene load(std::string const & path);

    explicit Scene(std::vector<Shape> && shapes);

    Scene(Scene const &) = delete;

    Scene(Scene && scene) noexcept;

    ~Scene();
};
