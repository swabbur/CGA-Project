#pragma once

#include <string>
#include <vector>

template<std::size_t>
class Vertex;

class Mesh {

    unsigned int ibo;
    unsigned int vbo;
    unsigned int vao;
    unsigned int size;
    bool textured;

    Mesh(unsigned int ibo, unsigned int vbo, unsigned int vao, unsigned int size, bool textured);

public:

    static Mesh create(std::vector<unsigned int> const & indices, std::vector<Vertex<3>> const & vertices, bool textured);

    static Mesh quad();

    Mesh(Mesh const &) = delete;

    Mesh(Mesh && mesh) noexcept;

    ~Mesh();

    [[nodiscard]] bool is_textured() const;

    void draw() const;
};
