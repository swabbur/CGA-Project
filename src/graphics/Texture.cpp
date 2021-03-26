#include <GL/glew.h>
#include <glm/glm.hpp>
#include <graphics/Texture.hpp>
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#include <stdexcept>

Texture Texture::load(std::string const & path) {

    int width;
    int height;
    int channels;

    stbi_set_flip_vertically_on_load(true);
    unsigned char * data = stbi_load(path.c_str(), &width, &height, &channels, STBI_rgb_alpha);
    if (data == nullptr) {
        throw std::runtime_error("Could not load texture: " + path);
    }

    GLuint handle;
    glCreateTextures(GL_TEXTURE_2D, 1, &handle);

    glTextureParameteri(handle, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTextureParameteri(handle, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glTextureParameteri(handle, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTextureParameteri(handle, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    if (glewIsExtensionSupported("GL_EXT_texture_filter_anisotropic")) {
        float anisotropy = 16.0f;
        glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &anisotropy);
        glTextureParameterf(handle, GL_TEXTURE_MAX_ANISOTROPY_EXT, anisotropy);
    }

    float size = std::min(width, height);
    int levels = std::max(1, static_cast<int>(glm::log2(size)));

    glTextureStorage2D(handle, levels, GL_RGBA8, width, height);
    glTextureSubImage2D(handle, 0, 0, 0, width, height, GL_RGBA, GL_UNSIGNED_BYTE, data);
    glGenerateTextureMipmap(handle);

    stbi_image_free(data);

    return Texture(Type::COLOR, handle);
}

Texture Texture::create(Type type, unsigned int width, unsigned int height) {

    GLuint handle;
    glCreateTextures(GL_TEXTURE_2D, 1, &handle);

    glTextureParameteri(handle, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTextureParameteri(handle, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTextureParameteri(handle, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTextureParameteri(handle, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    switch (type) {

        case Type::COLOR:
            glTextureStorage2D(handle, 1, GL_RGBA8, width, height);
            break;

        case Type::DEPTH:
            glTextureParameterf(handle, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_REF_TO_TEXTURE);
            glTextureStorage2D(handle, 1, GL_DEPTH_COMPONENT32F, width, height);
            break;

        default:
            throw std::runtime_error("Unsupported texture type");
    }

    return Texture(type, handle);
}

Texture::Texture(Type type, unsigned int handle) : type(type), handle(handle) {}

Texture::Texture(Texture && texture) noexcept : type(texture.type), handle(texture.handle) {
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
