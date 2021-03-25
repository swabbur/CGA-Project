#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <glm/glm.hpp>
#include <graphics/Model.hpp>
#include <graphics/Vertex.hpp>
#include <iostream>
#include <map>
#include <queue>

std::string get_filename(std::string const & path) {
    std::size_t position = path.find_last_of("[\\/]");
    return path.substr(position + 1);
}

glm::vec3 parse_vector(aiVector3D const & vector) {
    return glm::vec3(vector.x, vector.y, vector.z);
}

glm::vec3 parse_color(aiColor3D const & color) {
    return glm::vec3(color.r, color.g, color.b);
}

Material parse_material(aiMaterial const * assimp_material) {

    Material material;

    // Parse ambient color (map)
    if (assimp_material->GetTextureCount(aiTextureType_AMBIENT) > 0) {
        aiString assimp_filepath;
        assimp_material->GetTexture(aiTextureType_AMBIENT, 0, &assimp_filepath);
        std::string filepath = "textures/" + get_filename(assimp_filepath.C_Str());
        Texture texture = Texture::load(filepath);
        material.ambient.emplace<Texture>(std::move(texture));
    } else {
        aiColor3D assimp_color;
        assimp_material->Get(AI_MATKEY_COLOR_AMBIENT, assimp_color);
        glm::vec3 color = parse_color(assimp_color);
        material.ambient = color;
    }

    // Parse diffuse color (map)
    if (assimp_material->GetTextureCount(aiTextureType_DIFFUSE) > 0) {
        aiString assimp_filepath;
        assimp_material->GetTexture(aiTextureType_DIFFUSE, 0, &assimp_filepath);
        std::string filepath = "textures/" + get_filename(assimp_filepath.C_Str());
        Texture texture = Texture::load(filepath);
        material.diffuse.emplace<Texture>(std::move(texture));
    } else {
        aiColor3D assimp_color;
        assimp_material->Get(AI_MATKEY_COLOR_DIFFUSE, assimp_color);
        glm::vec3 color = parse_color(assimp_color);
        material.diffuse = color;
    }

    // Parse specular color (map)
    if (assimp_material->GetTextureCount(aiTextureType_SPECULAR) > 0) {
        aiString assimp_filepath;
        assimp_material->GetTexture(aiTextureType_SPECULAR, 0, &assimp_filepath);
        std::string filepath = "textures/" + get_filename(assimp_filepath.C_Str());
        Texture texture = Texture::load(filepath);
        material.specular.emplace<Texture>(std::move(texture));
    } else {
        aiColor3D assimp_color;
        assimp_material->Get(AI_MATKEY_COLOR_SPECULAR, assimp_color);
        glm::vec3 color = parse_color(assimp_color);
        material.specular = color;
    }

    // Parse shininess (map)
    if (assimp_material->GetTextureCount(aiTextureType_SHININESS) > 0) {
        aiString assimp_filepath;
        assimp_material->GetTexture(aiTextureType_SHININESS, 0, &assimp_filepath);
        std::string filepath = "textures/" + get_filename(assimp_filepath.C_Str());
        Texture texture = Texture::load(filepath);
        material.shininess.emplace<Texture>(std::move(texture));
    } else {
        float shininess;
        assimp_material->Get(AI_MATKEY_SHININESS, shininess);
        material.shininess = shininess;
    }

    // Parse normal map
    if (assimp_material->GetTextureCount(aiTextureType_NORMALS) > 0) {
        aiString assimp_filepath;
        assimp_material->GetTexture(aiTextureType_NORMALS, 0, &assimp_filepath);
        std::string filepath = "textures/" + get_filename(assimp_filepath.C_Str());
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
            throw std::runtime_error("Encountered a non-triangle face while loading model");
        }

        indices.push_back(static_cast<unsigned int>(face.mIndices[0]));
        indices.push_back(static_cast<unsigned int>(face.mIndices[1]));
        indices.push_back(static_cast<unsigned int>(face.mIndices[2]));
    }

    // Parse vertices
    std::vector<Vertex<3>> vertices;
    vertices.reserve(mesh->mNumVertices);
    bool textured = false;
    for (unsigned j = 0; j < mesh->mNumVertices; j++) {
        Vertex<3> vertex;
        vertex.position = glm::vec4(parse_vector(mesh->mVertices[j]), 1.0f);
        vertex.normal = parse_vector(mesh->mNormals[j]);
        if (mesh->HasTextureCoords(0)) {
            vertex.texture_coordinates = glm::vec2(parse_vector(mesh->mTextureCoords[0][j]));
            textured = true;
        }
        vertices.push_back(vertex);
    }

    // Parse AABB
    AABB const & aabb = AABB(glm::vec2(mesh->mAABB.mMin.x, mesh->mAABB.mMin.z), glm::vec2(mesh->mAABB.mMax.x, mesh->mAABB.mMax.z), mesh->mAABB.mMax.y - mesh->mAABB.mMin.y);

    // Create mesh
    return Mesh::create(indices, vertices, textured, aabb);
}

Model Model::load(std::string const & path) {

    // Load assimp model
    Assimp::Importer importer;
    aiScene const * scene = importer.ReadFile(path.c_str(),
                                              aiProcess_GenSmoothNormals
                                              | aiProcess_Triangulate
                                              | aiProcess_RemoveRedundantMaterials
                                              | aiProcess_PreTransformVertices
                                              | aiProcess_GenBoundingBoxes);
    if (scene == nullptr || scene->mRootNode == nullptr || scene->mFlags == AI_SCENE_FLAGS_INCOMPLETE) {
        throw std::runtime_error("Could not load model: " + path);
    }

    // Parse materials
    std::vector<Material> materials;
    materials.reserve(scene->mNumMaterials);
    for (unsigned int index = 0; index < scene->mNumMaterials; index++) {
        aiMaterial const * assimp_material = scene->mMaterials[index];
        Material material = parse_material(assimp_material);
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

    // Unload assimp model
    importer.FreeScene();

    return Model(std::move(materials), std::move(meshes), std::move(shapes));
}

Model::Model(std::vector<Material> && materials,
             std::vector<Mesh> && meshes,
             std::vector<Shape> && shapes) :
    materials(std::move(materials)),
    meshes(std::move(meshes)),
    shapes(std::move(shapes))
{}

Model::Model(Model && scene) noexcept :
    materials(std::move(scene.materials)),
    meshes(std::move(scene.meshes)),
    shapes(std::move(scene.shapes))
{}

Model::~Model() = default;
