#pragma once

#include <string>

class Texture {

    unsigned int handle;

    explicit Texture(unsigned int handle);

public:

    static Texture load(std::string const & path);

    Texture(Texture const &) = delete;

    Texture(Texture && texture) noexcept;

    ~Texture();

    void bind(unsigned int index) const;
};