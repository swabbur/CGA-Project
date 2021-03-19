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

    void set_bool(unsigned int location, bool boolean);

    void set_float(unsigned int location, float scalar);

    void set_vector(unsigned int location, glm::vec3 const & vector);

    void set_mat3(unsigned int location, glm::mat3 const& matrix);

    void set_mat4(unsigned int location, glm::mat4 const & matrix);

    void set_texture(unsigned int location, unsigned int index);
};