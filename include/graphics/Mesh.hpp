#pragma once

#include <string>
#include <vector>
#include <physics/AABB.hpp>

template<std::size_t>
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

    static Mesh create(std::vector<unsigned int> const & indices, std::vector<Vertex<3>> const & vertices, bool textured, AABB const & aabb);

    static Mesh quad();

    Mesh(Mesh const &) = delete;

    Mesh(Mesh && mesh) noexcept;

    ~Mesh();

    [[nodiscard]] bool is_textured() const;

    void draw() const;

    [[nodiscard]] AABB const & get_AABB() const;
};
