#include <devices/Window.hpp>
#include <graphics/Context.hpp>
#include <GL/glew.h>
#include <stdexcept>

Context::Context(Window & window) {

    window.make_context_current();

    auto glew_initialized = glewInit();
    if (glew_initialized != GLEW_OK) {
        throw std::runtime_error("Could not initialize GLEW");
    }
}

Context::~Context() = default;

void Context::set_view_port(unsigned int x, unsigned int y, unsigned int width, unsigned int height) { // NOLINT(readability-convert-member-functions-to-static)
    glViewport(x, y, width, height);
}

void Context::set_clear_color(float red, float green, float blue) { // NOLINT(readability-convert-member-functions-to-static)
    glClearColor(red, green, blue, 1.0f);
}

void Context::set_clear_depth(float depth) { // NOLINT(readability-convert-member-functions-to-static)
    glClearDepth(depth);
}

void Context::clear() { // NOLINT(readability-convert-member-functions-to-static)
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Context::set_multisampling(bool multisampling) {
    if (multisampling) {
        glEnable(GL_MULTISAMPLE);
    } else {
        glDisable(GL_MULTISAMPLE);
    }
}

void Context::set_depth_test(bool depth_test) { // NOLINT(readability-convert-member-functions-to-static)
    if (depth_test) {
        glEnable(GL_DEPTH_TEST);
    } else {
        glDisable(GL_DEPTH_TEST);
    }
}

void Context::set_cull_face(bool cull_face) { // NOLINT(readability-convert-member-functions-to-static)
    if (cull_face) {
        glEnable(GL_CULL_FACE);
    } else {
        glDisable(GL_CULL_FACE);
    }
}

void Context::set_alpha_blending(bool alpha_blending) { // NOLINT(readability-convert-member-functions-to-static)
    if (alpha_blending) {
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    } else {
        glDisable(GL_BLEND);
    }
}