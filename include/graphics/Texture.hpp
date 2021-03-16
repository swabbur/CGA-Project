#pragma once

#include <string>

class Texture {

public:

    enum class Type {
        COLOR,
        DEPTH,
    };

private:

    friend class Framebuffer;

    Type type;
    unsigned int handle;

    Texture(Type type, unsigned int handle);

public:

    static Texture load(std::string const & path);

    static Texture create(Type type, unsigned int width, unsigned int height);

    Texture(Texture const &) = delete;

    Texture(Texture && texture) noexcept;

    ~Texture();

    void bind(unsigned int index) const;
};