#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <glm/gtc/matrix_inverse.hpp>
#include <graphics/lights/AmbientLight.hpp>
#include <graphics/lights/DirectionalLight.hpp>
#include <graphics/lights/PointLight.hpp>
#include <graphics/lights/SpotLight.hpp>
#include <graphics/Scene.hpp>
#include <graphics/Vertex.hpp>
#include <iostream>
#include <map>
#include <queue>

std::string extract_directory(std::string const & filepath) {
    size_t position = filepath.find_last_of("[\\/]");
    if (position == std::string::npos) {
        return "";
    }
    return filepath.substr(0, position + 1);
}

glm::vec3 parse_vector(aiVector3D const & vector) {
    return glm::vec3(vector.x, vector.y, vector.z);
}

glm::mat4 parse_matrix(aiMatrix4x4 const & assimp_matrix) {
    glm::mat4 matrix;
    matrix[0][0] = assimp_matrix.a1;
    matrix[0][1] = assimp_matrix.b1;
    matrix[0][2] = assimp_matrix.c1;
    matrix[0][3] = assimp_matrix.d1;
    matrix[1][0] = assimp_matrix.a2;
    matrix[1][1] = assimp_matrix.b2;
    matrix[1][2] = assimp_matrix.c2;
    matrix[1][3] = assimp_matrix.d2;
    matrix[2][0] = assimp_matrix.a3;
    matrix[2][1] = assimp_matrix.b3;
    matrix[2][2] = assimp_matrix.c3;
    matrix[2][3] = assimp_matrix.d3;
    matrix[3][0] = assimp_matrix.a4;
    matrix[3][1] = assimp_matrix.b4;
    matrix[3][2] = assimp_matrix.c4;
    matrix[3][3] = assimp_matrix.d4;
    return matrix;
}

glm::vec3 parse_color(aiColor3D const & color) {
    return glm::vec3(color.r, color.g, color.b);
}

Material parse_material(std::string const & directory, aiMaterial const * assimp_material) {

    Material material;

    // PBR (Metallic/Roughness)

//    if (assimp_material->GetTextureCount(aiTextureType_BASE_COLOR) > 0) {
//        aiString assimp_filename;
//        assimp_material->GetTexture(aiTextureType_BASE_COLOR, 0, &assimp_filename);
//        std::string filename = assimp_filename.C_Str();
//        material.base_color.emplace<Texture>(Texture::load(directory + filename));
//    }
//
//    if (assimp_material->GetTextureCount(aiTextureType_METALNESS) > 0) {
//        aiString assimp_filename;
//        assimp_material->GetTexture(aiTextureType_METALNESS, 0, &assimp_filename);
//        std::string filename = assimp_filename.C_Str();
//        std::string filepath = directory + filename;
//        Texture texture = Texture::load(filepath);
//        material.metalness.emplace<Texture>(std::move(texture));
//    }
//
//    if (assimp_material->GetTextureCount(aiTextureType_DIFFUSE_ROUGHNESS) > 0) {
//        aiString assimp_filename;
//        assimp_material->GetTexture(aiTextureType_DIFFUSE_ROUGHNESS, 0, &assimp_filename);
//        std::string filename = assimp_filename.C_Str();
//        std::string filepath = directory + filename;
//        Texture texture = Texture::load(filepath);
//        material.roughness.emplace<Texture>(std::move(texture));
//    }
//
//    if (assimp_material->GetTextureCount(aiTextureType_AMBIENT_OCCLUSION) > 0) {
//        aiString assimp_filename;
//        assimp_material->GetTexture(aiTextureType_AMBIENT_OCCLUSION, 0, &assimp_filename);
//        std::string filename = assimp_filename.C_Str();
//        std::string filepath = directory + filename;
//        Texture texture = Texture::load(filepath);
//        material.ambient_occlusion.emplace<Texture>(std::move(texture));
//    }

    // Phong

    if (assimp_material->GetTextureCount(aiTextureType_AMBIENT) > 0) {
        aiString assimp_filename;
        assimp_material->GetTexture(aiTextureType_AMBIENT, 0, &assimp_filename);
        std::string filename = assimp_filename.C_Str();
        std::string filepath = directory + filename;
        Texture texture = Texture::load(filepath);
        material.ambient.emplace<Texture>(std::move(texture));
    } else {
        aiColor3D assimp_color;
        assimp_material->Get(AI_MATKEY_COLOR_AMBIENT, assimp_color);
        glm::vec3 color = parse_color(assimp_color);
        material.ambient = color;
    }

    if (assimp_material->GetTextureCount(aiTextureType_DIFFUSE) > 0) {
        aiString assimp_filename;
        assimp_material->GetTexture(aiTextureType_DIFFUSE, 0, &assimp_filename);
        std::string filename = assimp_filename.C_Str();
        std::string filepath = directory + filename;
        Texture texture = Texture::load(filepath);
        material.diffuse.emplace<Texture>(std::move(texture));
    } else {
        aiColor3D assimp_color;
        assimp_material->Get(AI_MATKEY_COLOR_DIFFUSE, assimp_color);
        glm::vec3 color = parse_color(assimp_color);
        material.diffuse = color;
    }

    if (assimp_material->GetTextureCount(aiTextureType_SPECULAR) > 0) {
        aiString assimp_filename;
        assimp_material->GetTexture(aiTextureType_SPECULAR, 0, &assimp_filename);
        std::string filename = assimp_filename.C_Str();
        std::string filepath = directory + filename;
        Texture texture = Texture::load(filepath);
        material.specular.emplace<Texture>(std::move(texture));
    } else {
        aiColor3D assimp_color;
        assimp_material->Get(AI_MATKEY_COLOR_SPECULAR, assimp_color);
        glm::vec3 color = parse_color(assimp_color);
        material.specular = color;
    }

    if (assimp_material->GetTextureCount(aiTextureType_SHININESS) > 0) {
        aiString assimp_filename;
        assimp_material->GetTexture(aiTextureType_SHININESS, 0, &assimp_filename);
        std::string filename = assimp_filename.C_Str();
        std::string filepath = directory + filename;
        Texture texture = Texture::load(filepath);
        material.shininess.emplace<Texture>(std::move(texture));
    } else {
        float shininess;
        assimp_material->Get(AI_MATKEY_SHININESS, shininess);
        material.shininess = shininess;
    }

    // Common

    if (assimp_material->GetTextureCount(aiTextureType_NORMALS) > 0) {
        aiString assimp_filename;
        assimp_material->GetTexture(aiTextureType_NORMALS, 0, &assimp_filename);
        std::string filename = assimp_filename.C_Str();
        std::string filepath = directory + filename;
        Texture texture = Texture::load(filepath);
        material.normal.emplace<Texture>(std::move(texture));
    }

    return std::move(material);
}

Mesh parse_mesh(aiMesh const * mesh) {

    // Parse triangle indices
    std::vector<unsigned int> indices;
    indices.reserve(3 * mesh->mNumFaces);
    for (unsigned j = 0; j < mesh->mNumFaces; j++) {

        aiFace const & face = mesh->mFaces[j];
        if (face.mNumIndices != 3) {
            throw std::runtime_error("Encountered a non-triangle face while loading scene");
        }

        indices.push_back(static_cast<unsigned int>(face.mIndices[0]));
        indices.push_back(static_cast<unsigned int>(face.mIndices[1]));
        indices.push_back(static_cast<unsigned int>(face.mIndices[2]));
    }

    // Parse vertices
    std::vector<Vertex> vertices;
    vertices.reserve(mesh->mNumVertices);
    bool textured = false;
    for (unsigned j = 0; j < mesh->mNumVertices; j++) {
        Vertex vertex;
        vertex.position = glm::vec4(parse_vector(mesh->mVertices[j]), 1.0f);
        vertex.normal = parse_vector(mesh->mNormals[j]);
        if (mesh->HasTextureCoords(0)) {
            vertex.texture_coordinates = glm::vec2(parse_vector(mesh->mTextureCoords[0][j]));
            textured = true;
        }
        vertices.push_back(vertex);
    }

    // Create mesh
    return Mesh::create(indices, vertices, textured);
}

Scene Scene::load(std::string const & path) {

    // Get directory from path
    std::string directory = extract_directory(path);

    // Load assimp scene
    Assimp::Importer importer;
    aiScene const * scene = importer.ReadFile(path.c_str(), aiProcess_GenSmoothNormals | aiProcess_Triangulate | aiProcess_RemoveRedundantMaterials | aiProcess_PreTransformVertices);
    if (scene == nullptr || scene->mRootNode == nullptr || scene->mFlags == AI_SCENE_FLAGS_INCOMPLETE) {
        throw std::runtime_error("Could not load scene: " + path);
    }

    // Parse lights
    Lights lights;
    for (unsigned int index = 0; index < scene->mNumLights; index++) {
        aiLight const * assimp_light = scene->mLights[index];
        switch (assimp_light->mType) {

            case aiLightSource_AMBIENT: {
                AmbientLight light;
                light.ambient = parse_color(assimp_light->mColorAmbient);
                light.diffuse = parse_color(assimp_light->mColorDiffuse);
                light.specular = parse_color(assimp_light->mColorSpecular);
                lights.ambient.push_back(light);
                break;
            }

            case aiLightSource_DIRECTIONAL: {
                DirectionalLight light;
                light.ambient = parse_color(assimp_light->mColorAmbient);
                light.diffuse = parse_color(assimp_light->mColorDiffuse);
                light.specular = parse_color(assimp_light->mColorSpecular);
                light.direction = parse_vector(assimp_light->mDirection);
                lights.directional.push_back(light);
                break;
            }

            case aiLightSource_POINT: {
                PointLight light;
                light.ambient = parse_color(assimp_light->mColorAmbient);
                light.diffuse = parse_color(assimp_light->mColorDiffuse);
                light.specular = parse_color(assimp_light->mColorSpecular);
                light.position = parse_vector(assimp_light->mPosition);
                light.attenuation.constant = assimp_light->mAttenuationConstant;
                light.attenuation.linear = assimp_light->mAttenuationLinear;
                light.attenuation.quadratic = assimp_light->mAttenuationQuadratic;
                lights.point.push_back(light);
                break;
            }

            case aiLightSource_SPOT: {
                SpotLight light;
                light.ambient = parse_color(assimp_light->mColorAmbient);
                light.diffuse = parse_color(assimp_light->mColorDiffuse);
                light.specular = parse_color(assimp_light->mColorSpecular);
                light.direction = parse_vector(assimp_light->mDirection);
                light.position = parse_vector(assimp_light->mPosition);
                light.attenuation.constant = assimp_light->mAttenuationConstant;
                light.attenuation.linear = assimp_light->mAttenuationLinear;
                light.attenuation.quadratic = assimp_light->mAttenuationQuadratic;
                light.angles.inner = assimp_light->mAngleInnerCone;
                light.angles.outer = assimp_light->mAngleOuterCone;
                lights.spot.push_back(light);
                break;
            }

            default:
                throw std::runtime_error("Unsupported lights type encountered");
        }
    }

    // Parse materials
    std::vector<Material> materials;
    materials.reserve(scene->mNumMaterials);
    for (unsigned int index = 0; index < scene->mNumMaterials; index++) {
        aiMaterial const * assimp_material = scene->mMaterials[index];
        Material material = parse_material(directory, assimp_material);
        materials.push_back(std::move(material));
    }

    // Parse meshes
    std::vector<Mesh> meshes;
    meshes.reserve(scene->mNumMeshes);
    std::vector<unsigned int> mesh_material_indices;
    mesh_material_indices.reserve(scene->mNumMeshes);
    for (unsigned int index = 0; index < scene->mNumMeshes; index++) {
        aiMesh const * assimp_mesh = scene->mMeshes[index];
        Mesh mesh = parse_mesh(assimp_mesh);
        meshes.push_back(std::move(mesh));
        mesh_material_indices.push_back(assimp_mesh->mMaterialIndex);
    }

    // Parse nodes
    std::vector<Shape> shapes;
    std::queue<aiNode const *> worklist;
    worklist.push(scene->mRootNode);
    while (!worklist.empty()) {
        aiNode const * node = worklist.front();
        worklist.pop();

        for (unsigned int index = 0; index < node->mNumMeshes; index++) {
            unsigned int mesh_index = node->mMeshes[index];
            unsigned int material_index = mesh_material_indices[mesh_index];
            Material & material = materials[material_index];
            Mesh & mesh = meshes[mesh_index];
            shapes.emplace_back(material, mesh);
        }

        for (unsigned index = 0; index < node->mNumChildren; index++) {
            worklist.push(node->mChildren[index]);
        }
    }

    // Unload assimp scene
    importer.FreeScene();

    return Scene(std::move(materials), std::move(meshes), std::move(shapes), std::move(lights));
}

Scene::Scene(std::vector<Material> && materials,
             std::vector<Mesh> && meshes,
             std::vector<Shape> && shapes,
             Lights && lights)
    : materials(std::move(materials)), meshes(std::move(meshes)), shapes(std::move(shapes)), lights(std::move(lights))
{}

Scene::Scene(Scene && scene) noexcept :
    shapes(std::move(scene.shapes))
{}

Scene::~Scene() = default;
