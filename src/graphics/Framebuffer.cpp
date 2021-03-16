#include <graphics/Framebuffer.hpp>
#include <graphics/Texture.hpp>
#include <GL/glew.h>
#include <stdexcept>

Framebuffer Framebuffer::get_default() {
    return Framebuffer(0);
}

Framebuffer Framebuffer::create(std::vector<std::reference_wrapper<Texture>> const & textures) {

    unsigned int handle;
    glCreateFramebuffers(1, &handle);

    unsigned int color_attachment_count = 0;
    unsigned int depth_attachment_count = 0;

    for (std::reference_wrapper<Texture> const & wrapper : textures) {
        Texture const & texture = wrapper.get();
        switch (texture.type) {

            case Texture::Type::COLOR: {
                unsigned int attachment = GL_COLOR_ATTACHMENT0 + color_attachment_count;
                glNamedFramebufferTexture(handle, attachment, texture.handle, 0);
                color_attachment_count++;
                break;
            }

            case Texture::Type::DEPTH: {

                if (depth_attachment_count == 1) {
                    throw std::runtime_error("Cannot attach multiple depth textures");
                }

                unsigned int attachment = GL_DEPTH_ATTACHMENT;
                glNamedFramebufferTexture(handle, attachment, texture.handle, 0);
                depth_attachment_count++;
                break;
            }

            default:
                throw std::runtime_error("Unsupported texture type used as framebuffer attachment");
        }
    }

    return Framebuffer(handle);
}

Framebuffer::Framebuffer(unsigned int handle): handle(handle) {}

Framebuffer::Framebuffer(Framebuffer && framebuffer) noexcept : handle(framebuffer.handle) {
    framebuffer.handle = 0;
}

Framebuffer::~Framebuffer() {
    if (handle != 0) {
        glDeleteFramebuffers(1, &handle);
    }
}

void Framebuffer::bind() const {
    glBindFramebuffer(GL_FRAMEBUFFER, handle);
}