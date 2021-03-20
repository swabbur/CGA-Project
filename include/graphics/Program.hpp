#pragma once

#include <glm/glm.hpp>
#include <string>
#include <vector>

class Shader;

class Program {

    unsigned int handle;

    explicit Program(unsigned int handle);

    [[nodiscard]] int get_uniform_location(std::string const & name) const;

public:

    static Program link(std::vector<Shader> const & shaders);

    static Program load(std::vector<std::string> const & paths);

    Program(Program const &) = delete;

    Program(Program && program) noexcept;

    ~Program();

    void bind() const;

    void set_bool(std::string const & name, bool boolean);

    void set_int(std::string const & name, int integer);

    void set_float(std::string const & name, float scalar);

    void set_vec3(std::string const & name, glm::vec3 const & vector);

    void set_mat3(std::string const & name, glm::mat3 const& matrix);

    void set_mat4(std::string const & name, glm::mat4 const & matrix);

    void set_sampler(std::string const & name, unsigned int index);
};