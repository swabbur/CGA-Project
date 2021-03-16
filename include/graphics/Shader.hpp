#pragma once

#include <string>

struct Shader {

    unsigned int handle;

    explicit Shader(unsigned int handle);

public:

    enum class Type {
        VERTEX,
        FRAGMENT,
    };

    static Shader compile(Type type, std::string const & source);

    static Shader load(std::string const & path);

    Shader(Shader const &) = delete;

    Shader(Shader && shader) noexcept;

    ~Shader();

    [[nodiscard]] unsigned int get_handle() const;
};
