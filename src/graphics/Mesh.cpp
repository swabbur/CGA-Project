#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <graphics/Mesh.hpp>
#include <GL/glew.h>
#include <glm/gtc/matrix_inverse.hpp>
#include <glm/mat3x3.hpp>
#include <glm/mat4x4.hpp>
#include <glm/vec4.hpp>
#include <stack>
#include <iostream>

Mesh Mesh::create(std::vector<unsigned int> const & indices, std::vector<float> const & vertices) {

    GLuint ibo;
    glCreateBuffers(1, &ibo);
    glNamedBufferStorage(ibo, indices.size() * sizeof(unsigned int), indices.data(), 0);

    GLuint vbo;
    glCreateBuffers(1, &vbo);
    glNamedBufferStorage(vbo, vertices.size() * sizeof(float), vertices.data(), 0);

    GLuint vao;
    glCreateVertexArrays(1, &vao);
    glVertexArrayElementBuffer(vao, ibo);
    glVertexArrayVertexBuffer(vao, 0, vbo, 0, 2 * sizeof(float));
    glVertexArrayAttribFormat(vao, 0, 2, GL_FLOAT, false, 0);
    glEnableVertexArrayAttrib(vao, 0);

    return Mesh(ibo, vbo, vao, indices.size());
}

Mesh Mesh::create_square() {

    std::vector<unsigned int> indices = {
            0, 1, 2,
            0, 2, 3,
    };

    std::vector<float> vertices = {
            0.0f, 0.0f,
            0.0f, 0.5f,
            0.5f, 0.5f,
            0.5f, 0.0f,
    };

    return Mesh::create(indices, vertices);
}

static glm::mat4 assimpMatrix(const aiMatrix4x4& m);
static glm::vec3 assimpVec(const aiVector3D& v);

struct Vertex {
    glm::vec3 pos;
    glm::vec3 normal;
    glm::vec2 texCoord;
};

Mesh Mesh::load(const std::string & path) {

    Assimp::Importer importer;
    aiScene const * scene = importer.ReadFile(path.c_str(), aiProcess_GenSmoothNormals | aiProcess_Triangulate);
    if (scene == nullptr || scene->mRootNode == nullptr || scene->mFlags == AI_SCENE_FLAGS_INCOMPLETE) {
        throw std::runtime_error("Could not load mesh: " + path);
    }

    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    bool has_texture_coordinates = false;

    std::stack<std::tuple<aiNode*, glm::mat4>> stack;
    stack.push({ scene->mRootNode, assimpMatrix(scene->mRootNode->mTransformation) });
    while (!stack.empty()) {
        auto [node, matrix] = stack.top();
        stack.pop();

        matrix *= assimpMatrix(node->mTransformation);
        const glm::mat3 normalMatrix = glm::inverseTranspose(glm::mat3(matrix));

        for (unsigned i = 0; i < node->mNumMeshes; i++) {

            // Process subMesh
            const aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
            if (mesh->mNumVertices == 0 || mesh->mNumFaces == 0) {
                std::cerr << "Skipping empty mesh" << std::endl;
                continue;
            }

            // Triangles
            const size_t indexOffset = vertices.size();
            for (unsigned j = 0; j < mesh->mNumFaces; j++) {

                const aiFace& face = mesh->mFaces[j];
                if (face.mNumIndices != 3) {
                    std::cerr << "Skipping non-triangle face" << std::endl;
                    continue;
                }

                auto aiIndices = face.mIndices;
                indices.push_back(static_cast<unsigned int>(aiIndices[0] + indexOffset));
                indices.push_back(static_cast<unsigned int>(aiIndices[1] + indexOffset));
                indices.push_back(static_cast<unsigned int>(aiIndices[2] + indexOffset));
            }

            // Vertices
            for (unsigned j = 0; j < mesh->mNumVertices; j++) {
                const glm::vec3 pos = matrix * glm::vec4(assimpVec(mesh->mVertices[j]), 1.0f);
                const glm::vec3 normal = normalMatrix * assimpVec(mesh->mNormals[j]);
                glm::vec2 texCoord { 0 };
                if (mesh->HasTextureCoords(0)) {
                    texCoord = glm::vec2(assimpVec(mesh->mTextureCoords[0][j]));
                    has_texture_coordinates = true;
                }
                vertices.push_back(Vertex { pos, normal, texCoord });
            }
        }

        for (unsigned i = 0; i < node->mNumChildren; i++) {
            stack.push({ node->mChildren[i], matrix });
        }
    }
    importer.FreeScene();

    // Create Element(/Index) Buffer Objects and Vertex Buffer Object.
    unsigned int ibo;
    glCreateBuffers(1, &ibo);
    glNamedBufferStorage(ibo, static_cast<GLsizeiptr>(indices.size() * sizeof(decltype(indices)::value_type)), indices.data(), 0);

    unsigned int vbo;
    glCreateBuffers(1, &vbo);
    glNamedBufferStorage(vbo, static_cast<GLsizeiptr>(vertices.size() * sizeof(Vertex)), vertices.data(), 0);

    // Bind vertex data to shader inputs using their index (location).
    // These bindings are stored in the Vertex Array Object.
    unsigned int vao;
    glCreateVertexArrays(1, &vao);

    // The indicies (pointing to vertices) should be read from the index buffer.
    glVertexArrayElementBuffer(vao, ibo);

    // The position and normal vectors should be retrieved from the specified Vertex Buffer Object.
    // The stride is the distance in bytes between vertices. We use the offset to point to the normals
    // instead of the positions.
    glVertexArrayVertexBuffer(vao, 0, vbo, offsetof(Vertex, pos), sizeof(Vertex));
    glVertexArrayVertexBuffer(vao, 1, vbo, offsetof(Vertex, normal), sizeof(Vertex));
    glVertexArrayVertexBuffer(vao, 2, vbo, offsetof(Vertex, texCoord), sizeof(Vertex));
    glEnableVertexArrayAttrib(vao, 0);
    glEnableVertexArrayAttrib(vao, 1);
    glEnableVertexArrayAttrib(vao, 2);

    unsigned int size = static_cast<GLsizei>(indices.size());

    // TODO: Add option for texture coordinates.

    return Mesh(ibo, vbo, vao, size);
}

static glm::mat4 assimpMatrix(const aiMatrix4x4& m) {
    glm::mat4 matrix;
    matrix[0][0] = m.a1;
    matrix[0][1] = m.b1;
    matrix[0][2] = m.c1;
    matrix[0][3] = m.d1;
    matrix[1][0] = m.a2;
    matrix[1][1] = m.b2;
    matrix[1][2] = m.c2;
    matrix[1][3] = m.d2;
    matrix[2][0] = m.a3;
    matrix[2][1] = m.b3;
    matrix[2][2] = m.c3;
    matrix[2][3] = m.d3;
    matrix[3][0] = m.a4;
    matrix[3][1] = m.b4;
    matrix[3][2] = m.c4;
    matrix[3][3] = m.d4;
    return matrix;
}

static glm::vec3 assimpVec(const aiVector3D& v) {
    return glm::vec3(v.x, v.y, v.z);
}

Mesh::Mesh(unsigned int ibo, unsigned int vbo, unsigned int vao, unsigned int size)
    : ibo(ibo), vbo(vbo), vao(vao), size(size) {}

Mesh::Mesh(Mesh && mesh) noexcept : ibo(mesh.ibo), vbo(mesh.vbo), vao(mesh.vao), size(mesh.size) {
    mesh.size = 0;
    mesh.vao = 0;
    mesh.vbo = 0;
    mesh.ibo = 0;
}

Mesh::~Mesh() {

    if (vao != 0) {
        glDeleteVertexArrays(1, &vao);
    }

    if (vbo != 0) {
        glDeleteBuffers(1, &vbo);
    }

    if (ibo != 0) {
        glDeleteBuffers(1, &ibo);
    }
}

void Mesh::draw() const {
    glBindVertexArray(vao);
    glDrawElements(GL_TRIANGLES, size, GL_UNSIGNED_INT, nullptr);
}