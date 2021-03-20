#pragma once

#include <string>
#include <vector>
#include <physics/AABB.hpp>

class Vertex;

class Mesh {

    unsigned int ibo;
    unsigned int vbo;
    unsigned int vao;
    unsigned int size;
    bool textured;
    AABB const aabb;

    Mesh(unsigned int ibo, unsigned int vbo, unsigned int vao, unsigned int size, bool textured, AABB const & aabb);

public:

    static Mesh create(std::vector<unsigned int> const & indices, std::vector<Vertex> const & vertices, bool textured, AABB const & aabb);

    Mesh(Mesh const &) = delete;

    Mesh(Mesh && mesh) noexcept;

    ~Mesh();

    [[nodiscard]] bool is_textured() const;

    void draw() const;

    AABB const & get_AABB() const;
};
