#include <graphics/Framebuffer.hpp>
#include <GL/glew.h>

Framebuffer Framebuffer::get_default() {
    return Framebuffer(0);
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