#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <glm/gtc/matrix_inverse.hpp>
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

glm::vec3 parse_color(aiColor4D const & color) {
    return glm::vec3(color.r, color.g, color.b);
}

Material parse_material(std::string const & directory, aiMaterial const * assimp_material) {

    Material material;

    if (assimp_material->GetTextureCount(aiTextureType_AMBIENT) > 0) {
        aiString assimp_filename;
        assimp_material->GetTexture(aiTextureType_AMBIENT, 0, &assimp_filename);
        std::string filename = assimp_filename.C_Str();
        material.ambient.emplace<Texture>(Texture::load(directory + filename));
    } else {
        aiColor4D ambient_color;
        assimp_material->Get(AI_MATKEY_COLOR_AMBIENT, ambient_color);
        material.ambient = parse_color(ambient_color);
    }

    if (assimp_material->GetTextureCount(aiTextureType_DIFFUSE) > 0) {
        aiString assimp_filename;
        assimp_material->GetTexture(aiTextureType_DIFFUSE, 0, &assimp_filename);
        std::string filename = assimp_filename.C_Str();
        material.diffuse.emplace<Texture>(Texture::load(directory + filename));
    } else {
        aiColor4D diffuse_color;
        assimp_material->Get(AI_MATKEY_COLOR_DIFFUSE, diffuse_color);
        material.diffuse = parse_color(diffuse_color);
    }

    if (assimp_material->GetTextureCount(aiTextureType_SPECULAR) > 0) {
        aiString assimp_filename;
        assimp_material->GetTexture(aiTextureType_SPECULAR, 0, &assimp_filename);
        std::string filename = assimp_filename.C_Str();
        material.specular.emplace<Texture>(Texture::load(directory + filename));
    } else {
        aiColor4D specular_color;
        assimp_material->Get(AI_MATKEY_COLOR_SPECULAR, specular_color);
        material.specular = parse_color(specular_color);
    }

    // TODO: Add shininess/roughness
    // TODO: Add normal map

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
        const glm::vec3 position = glm::vec4(parse_vector(mesh->mVertices[j]), 1.0f);
        const glm::vec3 normal = parse_vector(mesh->mNormals[j]);
        glm::vec2 texture_coordinates;
        if (mesh->HasTextureCoords(0)) {
            texture_coordinates = glm::vec2(parse_vector(mesh->mTextureCoords[0][j]));
            textured = true;
        }
        vertices.push_back({ position, normal, texture_coordinates });
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

    // TODO: Parse lights

    // Parse nodes
    std::vector<Shape> shapes;
    std::queue<aiNode const *> worklist;
    worklist.push(scene->mRootNode);
    while (!worklist.empty()) {
        aiNode const * node = worklist.front();
        worklist.pop();

        for (unsigned int index = 0; index < node->mNumMeshes; index++) {

            // Parse mesh
            aiMesh const * assimp_mesh = scene->mMeshes[node->mMeshes[index]];
            if (assimp_mesh->mNumVertices == 0 || assimp_mesh->mNumFaces == 0) {
                throw std::runtime_error("Encountered an empty mesh while loading scene");
            }
            Mesh mesh = parse_mesh(assimp_mesh);

            // Parse material
            aiMaterial const * assimp_material = scene->mMaterials[assimp_mesh->mMaterialIndex];
            Material material = parse_material(directory, assimp_material);

            shapes.emplace_back(std::move(material), std::move(mesh));
        }

        for (unsigned index = 0; index < node->mNumChildren; index++) {
            worklist.push(node->mChildren[index]);
        }
    }

    // Unload assimp scene
    importer.FreeScene();

    return Scene(std::move(shapes));
}

Scene::Scene(std::vector<Shape> && shapes) :
    shapes(std::move(shapes))
{}

Scene::Scene(Scene && scene) noexcept :
    shapes(std::move(scene.shapes))
{}

Scene::~Scene() = default;
