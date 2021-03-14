#pragma once

#include <string>
#include <vector>

class Vertex;

class Mesh {

    unsigned int ibo;
    unsigned int vbo;
    unsigned int vao;
    unsigned int size;
    bool textured;

    Mesh(unsigned int ibo, unsigned int vbo, unsigned int vao, unsigned int size, bool textured);

public:

    static Mesh create(std::vector<unsigned int> const & indices, std::vector<Vertex> const & vertices, bool textured);

    Mesh(Mesh const &) = delete;

    Mesh(Mesh && mesh) noexcept;

    ~Mesh();

    [[nodiscard]] bool is_textured() const;

    void draw() const;
};
