#include <GL/glew.h>
#include <glm/gtc/matrix_inverse.hpp>
#include <glm/mat3x3.hpp>
#include <graphics/Mesh.hpp>
#include <graphics/Vertex.hpp>
#include <iostream>

Mesh Mesh::create(std::vector<unsigned int> const & indices, std::vector<Vertex<3>> const & vertices, bool textured, AABB const& aabb) {

    GLuint ibo;
    glCreateBuffers(1, &ibo);
    glNamedBufferStorage(ibo, indices.size() * sizeof(unsigned int), indices.data(), 0);

    GLuint vbo;
    glCreateBuffers(1, &vbo);
    glNamedBufferStorage(vbo, vertices.size() * sizeof(Vertex<3>), vertices.data(), 0);

    unsigned int vao;
    glCreateVertexArrays(1, &vao);
    glVertexArrayElementBuffer(vao, ibo);
    glVertexArrayVertexBuffer(vao, 0, vbo, offsetof(Vertex<3>, position), sizeof(Vertex<3>));
    glVertexArrayVertexBuffer(vao, 1, vbo, offsetof(Vertex<3>, normal), sizeof(Vertex<3>));
    glVertexArrayVertexBuffer(vao, 2, vbo, offsetof(Vertex<3>, texture_coordinates), sizeof(Vertex<3>));
    glVertexArrayAttribFormat(vao, 0, 3, GL_FLOAT, false, 0);
    glVertexArrayAttribFormat(vao, 1, 3, GL_FLOAT, false, 0);
    glVertexArrayAttribFormat(vao, 2, 2, GL_FLOAT, false, 0);
    glEnableVertexArrayAttrib(vao, 0);
    glEnableVertexArrayAttrib(vao, 1);
    glEnableVertexArrayAttrib(vao, 2);

    unsigned int size = indices.size();

    return Mesh(ibo, vbo, vao, size, textured, aabb);
}

Mesh Mesh::quad() {

    std::vector<unsigned int> indices = {
        0, 1, 2,
        0, 2, 3
    };

    std::vector<Vertex<2>> vertices = {
            Vertex<2>(glm::vec2(-1.0f, 1.0f), glm::vec2(0.0f, 1.0f)),
            Vertex<2>(glm::vec2(-1.0f, -1.0f), glm::vec2(0.0f, 0.0f)),
            Vertex<2>(glm::vec2(1.0f, -1.0f), glm::vec2(1.0f, 0.0f)),
            Vertex<2>(glm::vec2(1.0f, 1.0f), glm::vec2(1.0f, 1.0f)),
    };

    GLuint ibo;
    glCreateBuffers(1, &ibo);
    glNamedBufferStorage(ibo, indices.size() * sizeof(unsigned int), indices.data(), 0);

    GLuint vbo;
    glCreateBuffers(1, &vbo);
    glNamedBufferStorage(vbo, vertices.size() * sizeof(Vertex<2>), vertices.data(), 0);

    unsigned int vao;
    glCreateVertexArrays(1, &vao);
    glVertexArrayElementBuffer(vao, ibo);
    glVertexArrayVertexBuffer(vao, 0, vbo, offsetof(Vertex<2>, position), sizeof(Vertex<2>));
    glVertexArrayVertexBuffer(vao, 1, vbo, offsetof(Vertex<2>, texture_coordinates), sizeof(Vertex<2>));
    glVertexArrayAttribFormat(vao, 0, 2, GL_FLOAT, false, 0);
    glVertexArrayAttribFormat(vao, 1, 2, GL_FLOAT, false, 0);
    glEnableVertexArrayAttrib(vao, 0);
    glEnableVertexArrayAttrib(vao, 1);

    unsigned int size = indices.size();

    AABB bounding_box(glm::vec2(0.0f, 0.0f), glm::vec2(0.0f, 0.0f), 0.0f);

    return Mesh(ibo, vbo, vao, size, true, bounding_box);
}

Mesh::Mesh(unsigned int ibo, unsigned int vbo, unsigned int vao, unsigned int size, bool textured, AABB const & aabb)
    : ibo(ibo), vbo(vbo), vao(vao), size(size), textured(textured), aabb(aabb) {}

Mesh::Mesh(Mesh && mesh) noexcept :
    ibo(mesh.ibo), vbo(mesh.vbo), vao(mesh.vao), size(mesh.size), textured(mesh.textured), aabb(mesh.aabb)
{
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

bool Mesh::is_textured() const {
    return textured;
}

void Mesh::draw() const {
    glBindVertexArray(vao);
    glDrawElements(GL_TRIANGLES, size, GL_UNSIGNED_INT, nullptr);
}

AABB const& Mesh::get_AABB() const {
    return aabb;
}