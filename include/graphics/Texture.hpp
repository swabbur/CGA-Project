#pragma once

#include <string>

class Texture {

    unsigned int handle;

    explicit Texture(unsigned int handle);

public:

    enum class Type {
        COLOR,
        DEPTH,
    };

    static Texture load_rgb(std::string const & path);

    static Texture load_grey(std::string const & path);

    static Texture create(Type type, unsigned int width, unsigned int height);

    Texture(Texture const &) = delete;

    Texture(Texture && texture) noexcept;

    ~Texture();

    void bind(unsigned int index) const;
};