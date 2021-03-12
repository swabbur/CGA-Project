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

void Program::set_mat4(unsigned int location, glm::mat4 const & matrix) { // NOLINT(readability-convert-member-functions-to-static)
    glUniformMatrix4fv(location, 1, false, glm::value_ptr(matrix));
}

void Program::set_texture(unsigned int location, unsigned int index) { // NOLINT(readability-convert-member-functions-to-static)
    glUniform1i(location, index);
}
