#pragma once

#include <string>

template<typename, typename>
class Cache;

class Texture {

    static Cache<std::string, Texture> cache;

    unsigned int handle;

    explicit Texture(unsigned int handle);

public:

    static Texture & get(std::string const & path);

    static Texture load(std::string const & path);

    Texture(Texture const &) = delete;

    Texture(Texture && texture) noexcept;

    ~Texture();

    void bind(unsigned int index) const;
};