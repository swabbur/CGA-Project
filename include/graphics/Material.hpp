#pragma once

#include "Texture.hpp"

#include <glm/glm.hpp>
#include <variant>

// Based on materials from https://freepbr.com
// and the gltf exporter https://docs.blender.org/manual/en/latest/addons/import_export/scene_gltf2.html.

class Material {

public:

//    // PBR (Metallic/Roughness)
//    std::variant<std::monostate, glm::vec3, Texture> base_color;
//    std::variant<std::monostate, float, Texture> metalness;
//    std::variant<std::monostate, float, Texture> roughness;
//    std::variant<std::monostate, float, Texture> ambient_occlusion;
//    std::variant<std::monostate, Texture> normal;

    // Phong
    std::variant<std::monostate, glm::vec3, Texture> ambient;
    std::variant<std::monostate, glm::vec3, Texture> diffuse;
    std::variant<std::monostate, glm::vec3, Texture> specular;
    std::variant<std::monostate, float, Texture> shininess;
    std::variant<std::monostate, Texture> normal;

    Material();
};
