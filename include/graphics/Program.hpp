#pragma once

#include <glm/glm.hpp>
#include <string>
#include <vector>

class Shader;

class Program {

    unsigned int handle;

    explicit Program(unsigned int handle);

public:

    static Program link(std::vector<Shader> const & shaders);

    static Program load(std::vector<std::string> const & paths);

    Program(Program const &) = delete;

    Program(Program && program) noexcept;

    ~Program();

    void bind() const;

    void set_matrix(unsigned int location, glm::mat4 const & matrix);

    void set_texture(unsigned int location, unsigned int index);
};