#pragma once

#include <string>
#include <vector>

class Mesh {

    unsigned int ibo;
    unsigned int vbo;
    unsigned int vao;
    unsigned int size;

    Mesh(unsigned int ibo, unsigned int vbo, unsigned int vao, unsigned int size);

public:

    static Mesh create(std::vector<unsigned int> const & indices, std::vector<float> const & vertices);

    static Mesh create_square();

    static Mesh load(std::string const & path);

    Mesh(Mesh const &) = delete;

    Mesh(Mesh && mesh) noexcept;

    ~Mesh();

    void draw() const;
};
