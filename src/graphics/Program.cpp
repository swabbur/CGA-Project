#include <GL/glew.h>
#include <glm/gtc/type_ptr.hpp>
#include <graphics/Program.hpp>
#include <graphics/Shader.hpp>
#include <stdexcept>

Program Program::link(std::vector<Shader> const & shaders) {

    // Create program
    GLuint handle = glCreateProgram();

    // Link program
    for (Shader const & shader : shaders) {
        glAttachShader(handle, shader.get_handle());
    }
    glLinkProgram(handle);

    // Verify linking
    GLint linked;
    glGetProgramiv(handle, GL_LINK_STATUS, &linked);
    if (!linked) {

        GLint log_length;
        glGetProgramiv(handle, GL_INFO_LOG_LENGTH, &log_length);

        std::string log;
        log.resize(log_length);
        glGetProgramInfoLog(handle, log_length, nullptr, log.data());

        throw std::runtime_error("Could not link program:\n" + log);
    }

    return Program(handle);
}

Program Program::load(std::vector<std::string> const & paths) {

    // Load shaders
    std::vector<Shader> shaders;
    for (std::string const & path : paths) {
        Shader shader = Shader::load(path);
        shaders.push_back(std::move(shader));
    }

    // Link program
    return link(shaders);
}

Program::Program(unsigned int handle) : handle(handle) {}

Program::Program(Program && program) noexcept : handle(program.handle) {
    program.handle = 0;
}

Program::~Program() {
    if (handle != 0) {
        glDeleteProgram(handle);
    }
}

void Program::bind() const {
    glUseProgram(handle);
}

int Program::get_uniform_location(std::string const & name) const {
    return glGetUniformLocation(handle, name.c_str());
}

void Program::set_bool(unsigned int location, bool boolean) { // NOLINT(readability-convert-member-functions-to-static)
    glUniform1ui(location, boolean);
}

void Program::set_float(unsigned int location, float scalar) { // NOLINT(readability-convert-member-functions-to-static)
    glUniform1f(location, scalar);
}

void Program::set_vec3(unsigned int location, glm::vec3 const & vector) { // NOLINT(readability-convert-member-functions-to-static)
    glUniform3fv(location, 1, glm::value_ptr(vector));
}

void Program::set_mat3(unsigned int location, glm::mat3 const& matrix) { // NOLINT(readability-convert-member-functions-to-static)
    glUniformMatrix3fv(location, 1, false, glm::value_ptr(matrix));
}

void Program::set_mat4(unsigned int location, glm::mat4 const & matrix) { // NOLINT(readability-convert-member-functions-to-static)
    glUniformMatrix4fv(location, 1, false, glm::value_ptr(matrix));
}

void Program::set_sampler(unsigned int location, unsigned int index) { // NOLINT(readability-convert-member-functions-to-static)
    glUniform1i(location, index);
}

void Program::set_bool(std::string const & name, bool boolean) {
    int location = get_uniform_location(name);
    set_bool(location, boolean);
}

void Program::set_float(std::string const & name, float scalar) {
    int location = get_uniform_location(name);
    set_float(location, scalar);
}

void Program::set_vec3(std::string const & name, glm::vec3 const & vector) {
    int location = get_uniform_location(name);
    set_vec3(location, vector);
}

void Program::set_mat3(std::string const & name, glm::mat3 const & matrix) {
    int location = get_uniform_location(name);
    set_mat3(location, matrix);
}

void Program::set_mat4(std::string const & name, glm::mat4 const & matrix) {
    int location = get_uniform_location(name);
    set_mat4(location, matrix);
}

void Program::set_sampler(std::string const & name, unsigned int index) {
    int location = get_uniform_location(name);
    set_sampler(location, index);
}