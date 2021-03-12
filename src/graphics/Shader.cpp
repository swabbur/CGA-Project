#include <graphics/Shader.hpp>
#include <GL/glew.h>
#include <util/Files.hpp>
#include <stdexcept>

Shader Shader::compile(Shader::Type type, std::string const & source) {

    // Create handle
    GLuint handle;
    switch (type) {

        case Type::VERTEX:
            handle = glCreateShader(GL_VERTEX_SHADER);
            break;

        case Type::FRAGMENT:
            handle = glCreateShader(GL_FRAGMENT_SHADER);
            break;

        default:
            throw std::runtime_error("Unsupported handle type");
    }

    // Store and compile handle source
    char const * source_pointer = source.c_str();
    glShaderSource(handle, 1, &source_pointer, nullptr);
    glCompileShader(handle);

    // Verify compilation
    GLint compiled;
    glGetShaderiv(handle, GL_COMPILE_STATUS, &compiled);
    if (!compiled) {

        GLint log_length;
        glGetShaderiv(handle, GL_INFO_LOG_LENGTH, &log_length);

        std::string log;
        log.resize(log_length);
        glGetShaderInfoLog(handle, log_length, nullptr, log.data());

        glDeleteShader(handle);

        throw std::runtime_error("Could not compile shader:\n" + log);
    }

    return Shader(handle);
}

bool contains(std::string const & string, std::string const & sub_string) {
    return string.find(sub_string) != std::string::npos;
}

Shader::Type get_type_from_path(std::string const & path) {

    if (contains(path, "vert")) {
        return Shader::Type::VERTEX;
    }

    if (contains(path, "frag")) {
        return Shader::Type::FRAGMENT;
    }

    throw std::runtime_error("Could not determine shader type");
}

Shader Shader::load(std::string const & path) {

    // Determine handle type
    Shader::Type type = get_type_from_path(path);

    // Load handle source
    std::string source = Files::read_text(path);

    return compile(type, source);
}

Shader::Shader(unsigned int handle) : handle(handle) {}

Shader::Shader(Shader && shader) noexcept : handle(shader.handle) {
    shader.handle = 0;
}

Shader::~Shader() {
    if (handle != 0) {
        glDeleteShader(handle);
    }
}

[[nodiscard]] unsigned int Shader::get_handle() const {
    return handle;
}
