#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <glm/gtc/matrix_inverse.hpp>
#include <graphics/Scene.hpp>
#include <graphics/Vertex.hpp>
#include <stack>

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

glm::vec3 parse_vector(const aiVector3D& v) {
    return glm::vec3(v.x, v.y, v.z);
}

Scene Scene::load(std::string const & path) {

    // Load assimp scene
    Assimp::Importer importer;
    aiScene const * scene = importer.ReadFile(path.c_str(), aiProcess_GenSmoothNormals | aiProcess_Triangulate);
    if (scene == nullptr || scene->mRootNode == nullptr || scene->mFlags == AI_SCENE_FLAGS_INCOMPLETE) {
        throw std::runtime_error("Could not load scene: " + path);
    }

    // Parse textures
    std::vector<Texture> textures;
    textures.reserve(scene->mNumTextures);
    for (unsigned int index = 0; index < scene->mNumTextures; index++) {
        aiTexture const * assimp_texture = scene->mTextures[index];
        std::string const filename = assimp_texture->mFilename.data;
        Texture texture = Texture::load(filename);
        textures.emplace_back(std::move(texture));
    }

    // Parse materials
    std::vector<Material> materials;
    materials.reserve(scene->mNumMaterials);
    for (unsigned int index = 0; index < scene->mNumMaterials; index++) {
        aiMaterial const * assimp_material = scene->mMaterials[index];
        // TODO: Parse material
        Material material;
        materials.emplace_back(material);
    }

    // Parse meshes
    std::vector<Mesh> meshes;
    std::vector<unsigned int> material_indices;
    meshes.reserve(scene->mNumMeshes);
    for (unsigned int index = 0; index < scene->mNumMeshes; index++) {

        // Validate mesh integrity
        aiMesh const * assimp_mesh = scene->mMeshes[index];
        if (assimp_mesh->mNumVertices == 0 || assimp_mesh->mNumFaces == 0) {
            throw std::runtime_error("Encountered an empty mesh while loading scene");
        }

        // Parse triangle indices
        std::vector<unsigned int> indices;
        indices.reserve(3 * assimp_mesh->mNumFaces);
        for (unsigned j = 0; j < assimp_mesh->mNumFaces; j++) {

            aiFace const & face = assimp_mesh->mFaces[j];
            if (face.mNumIndices != 3) {
                throw std::runtime_error("Encountered a non-triangle face while loading scene");
            }

            indices.push_back(static_cast<unsigned int>(face.mIndices[0]));
            indices.push_back(static_cast<unsigned int>(face.mIndices[1]));
            indices.push_back(static_cast<unsigned int>(face.mIndices[2]));
        }

        // Parse vertices
        std::vector<Vertex> vertices;
        vertices.reserve(assimp_mesh->mNumVertices);
        bool textured = false;
        for (unsigned j = 0; j < assimp_mesh->mNumVertices; j++) {
            const glm::vec3 position = glm::vec4(parse_vector(assimp_mesh->mVertices[j]), 1.0f);
            const glm::vec3 normal = parse_vector(assimp_mesh->mNormals[j]);
            glm::vec2 texture_coordinates;
            if (assimp_mesh->HasTextureCoords(0)) {
                texture_coordinates = glm::vec2(parse_vector(assimp_mesh->mTextureCoords[0][j]));
                textured = true;
            }
            vertices.push_back({ position, normal, texture_coordinates });
        }

        // Create mesh
        Mesh mesh = Mesh::create(indices, vertices, textured);
        meshes.emplace_back(std::move(mesh));
    }

    // Parse lights
    // TODO: Parse lights

    // Parse nodes
    std::vector<Shape> shapes;

    std::stack<std::tuple<aiNode*, glm::mat4>> stack;
    stack.push({ scene->mRootNode, parse_matrix(scene->mRootNode->mTransformation) });
    while (!stack.empty()) {
        auto [node, transformation] = stack.top();
        stack.pop();

        transformation *= parse_matrix(node->mTransformation);

        for (unsigned int index = 0; index < node->mNumMeshes; index++) {
            unsigned int mesh_index = node->mMeshes[index];
            Material const & material = materials[material_indices[mesh_index]];
            Mesh const & mesh = meshes[mesh_index];
            shapes.emplace_back(material, mesh, transformation);
        }

        for (unsigned index = 0; index < node->mNumChildren; index++) {
            stack.push({ node->mChildren[index], transformation });
        }
    }

    // Unload assimp scene
    importer.FreeScene();

    // Create scene
    return Scene(std::move(textures), std::move(materials), std::move(meshes), std::move(shapes));
}

Scene::Scene(std::vector<Texture> && textures,
             std::vector<Material> && materials,
             std::vector<Mesh> && meshes,
             std::vector<Shape> && shapes) :
    textures(textures),
    materials(materials),
    meshes(meshes),
    shapes(shapes)
{}

Scene::Scene(Scene && scene) noexcept :
        textures(std::move(scene.textures)),
        materials(std::move(scene.materials)),
        meshes(std::move(scene.meshes)),
        shapes(std::move(scene.shapes))
{}

Scene::~Scene() = default;
