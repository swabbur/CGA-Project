#include <GL/glew.h>
#include <graphics/Texture.hpp>
#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>
#include <iostream>

Texture Texture::load_rgb(std::string const & path) {

    int width;
    int height;
    int channels;

    stbi_set_flip_vertically_on_load(true);
    unsigned char * data = stbi_load(path.c_str(), &width, &height, &channels, STBI_rgb);
    if (data == nullptr) {
        throw std::runtime_error("Could not load_rgb texture: " + path);
    }

    GLuint handle;
    glCreateTextures(GL_TEXTURE_2D, 1, &handle);

    glTextureParameteri(handle, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTextureParameteri(handle, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glTextureParameteri(handle, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTextureParameteri(handle, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);

    glTextureStorage2D(handle, 1, GL_RGB8, width, height);
    glTextureSubImage2D(handle, 0, 0, 0, width, height, GL_RGB, GL_UNSIGNED_BYTE, data);

    glGenerateTextureMipmap(handle);

    stbi_image_free(data);

    return Texture(handle);
}

Texture Texture::load_grey(std::string const & path) {

    int width;
    int height;
    int channels;

    stbi_set_flip_vertically_on_load(true);
    unsigned char * data = stbi_load(path.c_str(), &width, &height, &channels, STBI_grey);
    if (data == nullptr) {
        throw std::runtime_error("Could not load_rgb texture: " + path);
    }

    GLuint handle;
    glCreateTextures(GL_TEXTURE_2D, 1, &handle);

    glTextureParameteri(handle, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTextureParameteri(handle, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glTextureParameteri(handle, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTextureParameteri(handle, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);

    glTextureStorage2D(handle, 1, GL_R8, width, height);
    glTextureSubImage2D(handle, 0, 0, 0, width, height, GL_R, GL_UNSIGNED_BYTE, data);

    glGenerateTextureMipmap(handle);

    stbi_image_free(data);

    return Texture(handle);
}

Texture Texture::create(Type type, unsigned int width, unsigned int height) {

    GLuint handle;
    glCreateTextures(GL_TEXTURE_2D, 1, &handle);

    glTextureParameteri(handle, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTextureParameteri(handle, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    switch (type) {

        case Type::COLOR:
            glTextureParameteri(handle, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTextureParameteri(handle, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glTextureStorage2D(handle, 1, GL_RGBA8, width, height);
            break;

        case Type::DEPTH:
            glTextureParameteri(handle, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
            glTextureParameteri(handle, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
            glTextureStorage2D(handle, 1, GL_DEPTH_COMPONENT32F, width, height);
            break;

        default:
            throw std::runtime_error("Unsupported texture type");
    }

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
