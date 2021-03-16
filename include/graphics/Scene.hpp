#pragma once

#include "lights/AmbientLight.hpp"
#include "lights/DirectionalLight.hpp"
#include "lights/PointLight.hpp"
#include "lights/SpotLight.hpp"
#include "Material.hpp"
#include "Mesh.hpp"
#include "Shape.hpp"
#include "Texture.hpp"

#include <string>
#include <vector>

class Scene {

public:

    struct Lights {
        std::vector<AmbientLight> ambient;
        std::vector<DirectionalLight> directional;
        std::vector<PointLight> point;
        std::vector<SpotLight> spot;
    };

    static Scene load(std::string const & path);

    std::vector<Material> materials;
    std::vector<Mesh> meshes;
    std::vector<Shape> shapes;
    Lights lights;

    explicit Scene(std::vector<Material> && materials,
                   std::vector<Mesh> && meshes,
                   std::vector<Shape> && shapes,
                   Lights && lights);

    Scene(Scene const &) = delete;

    Scene(Scene && scene) noexcept;

    ~Scene();
};
