#include <GL/glew.h>
#include <glm/gtc/matrix_inverse.hpp>
#include <glm/mat3x3.hpp>
#include <graphics/Mesh.hpp>
#include <graphics/Vertex.hpp>

Mesh Mesh::create(std::vector<unsigned int> const & indices, std::vector<Vertex> const & vertices, bool textured) {

    GLuint ibo;
    glCreateBuffers(1, &ibo);
    glNamedBufferStorage(ibo, indices.size() * sizeof(unsigned int), indices.data(), 0);

    GLuint vbo;
    glCreateBuffers(1, &vbo);
    glNamedBufferStorage(vbo, vertices.size() * sizeof(Vertex), vertices.data(), 0);

    unsigned int vao;
    glCreateVertexArrays(1, &vao);
    glVertexArrayElementBuffer(vao, ibo);
    glVertexArrayVertexBuffer(vao, 0, vbo, offsetof(Vertex, position), sizeof(Vertex));
    glVertexArrayVertexBuffer(vao, 1, vbo, offsetof(Vertex, normal), sizeof(Vertex));
    glVertexArrayVertexBuffer(vao, 2, vbo, offsetof(Vertex, texture_coordinates), sizeof(Vertex));
    glVertexArrayAttribFormat(vao, 0, 3, GL_FLOAT, false, 0);
    glVertexArrayAttribFormat(vao, 1, 3, GL_FLOAT, false, 0);
    glVertexArrayAttribFormat(vao, 2, 2, GL_FLOAT, false, 0);
    glEnableVertexArrayAttrib(vao, 0);
    glEnableVertexArrayAttrib(vao, 1);
    glEnableVertexArrayAttrib(vao, 2);

    unsigned int size = indices.size();

    return Mesh(ibo, vbo, vao, size, textured);
}

Mesh::Mesh(unsigned int ibo, unsigned int vbo, unsigned int vao, unsigned int size, bool textured)
    : ibo(ibo), vbo(vbo), vao(vao), size(size), textured(textured) {}

Mesh::Mesh(Mesh && mesh) noexcept :
    ibo(mesh.ibo), vbo(mesh.vbo), vao(mesh.vao), size(mesh.size), textured(mesh.textured)
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