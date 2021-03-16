#include <GL/glew.h>
#include <graphics/Texture.hpp>
#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>
#include <iostream>
#include <util/Cache.hpp>

Cache<std::string, Texture> Texture::cache(Texture::load);

Texture & Texture::get(std::string const & path) {
    return cache.get(path);
}

Texture Texture::load(std::string const & path) {

    int width;
    int height;
    int channels;
    unsigned char * data = stbi_load(path.c_str(), &width, &height, &channels, 4);
    if (data == nullptr) {
        throw std::runtime_error("Could not load texture: " + path);
    }

    GLuint handle;
    glCreateTextures(GL_TEXTURE_2D, 1, &handle);
    glTextureStorage2D(handle, 1, GL_RGBA8, width, height);
    glTextureSubImage2D(handle, 0, 0, 0, width, height, GL_RGBA, GL_UNSIGNED_BYTE, data);

    glTextureParameteri(handle, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTextureParameteri(handle, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glTextureParameteri(handle, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTextureParameteri(handle, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    stbi_image_free(data);

    return Texture(handle);
}

Texture::Texture(unsigned int handle) : handle(handle) {}

Texture::Texture(Texture && texture) noexcept : handle(texture.handle) {
    texture.handle = 0;
}

Texture::~Texture() {
    if (handle != 0) {
        glDeleteTextures(1, &handle);
    }
}

void Texture::bind(unsigned int index) const {
    glActiveTexture(GL_TEXTURE0 + index);
    glBindTexture(GL_TEXTURE_2D, handle);
}
