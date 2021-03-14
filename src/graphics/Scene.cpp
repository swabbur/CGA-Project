#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <glm/gtc/matrix_inverse.hpp>
#include <graphics/Scene.hpp>
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
    }

    // Parse meshes
    std::vector<Mesh> meshes;
    std::vector<unsigned int> material_indices;
    meshes.reserve(scene->mNumMeshes);
    for (unsigned int index = 0; index < scene->mNumMeshes; index++) {
        aiMesh const * assimp_mesh = scene->mMeshes[index];
        // TODO: Parse mesh
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
        glm::mat3 const normalMatrix = glm::inverseTranspose(glm::mat3(transformation));

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
