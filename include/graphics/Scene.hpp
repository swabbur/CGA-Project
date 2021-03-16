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

template<typename, typename>
class Cache;

class Scene {

    static Cache<std::string, Scene> cache;

public:

    std::vector<Shape> shapes;
    struct Lights {
        std::vector<AmbientLight> ambient;
        std::vector<DirectionalLight> directional;
        std::vector<PointLight> point;
        std::vector<SpotLight> spot;
    } lights;

    static Scene load(std::string const & path);

    static Scene & get(std::string const & path);

    explicit Scene(std::vector<Shape> && shapes, Lights && lights);

    Scene(Scene const &) = delete;

    Scene(Scene && scene) noexcept;

    ~Scene();
};
