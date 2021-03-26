#include <devices/Window.hpp>
#include <GLFW/glfw3.h>

Window::Window() : handle(nullptr), mutex(), latest{}, polled(latest), resized(false) {}

Window::~Window() = default;

void Window::set_handle(void * new_handle) {
    handle = new_handle;
}

bool Window::is_closed() const {
    return polled.closed;
}

int Window::get_width() const {
    return polled.width;
}

int Window::get_height() const {
    return polled.height;
}

float Window::get_aspect_ratio() const {
    return static_cast<float>(polled.width) / static_cast<float>(polled.height);
}

bool Window::is_resized() const {
    return resized;
}

void Window::resize(int width, int height) {
    std::scoped_lock<std::mutex> lock(mutex);
    latest.width = width;
    latest.height = height;
}

void Window::close() {
    std::scoped_lock<std::mutex> lock(mutex);
    latest.closed = true;
}

void Window::make_context_current() {

    if (!handle) {
        throw std::runtime_error("GLFW handle was not set");
    }

    glfwMakeContextCurrent(static_cast<GLFWwindow *>(handle));
}

void Window::swap_buffers() {

    if (!handle) {
        throw std::runtime_error("GLFW handle was not set");
    }

    glfwSwapBuffers(static_cast<GLFWwindow *>(handle));
}

void Window::poll() {
    std::scoped_lock<std::mutex> lock(mutex);
    resized = (polled.width != latest.width) || (polled.height != latest.height);
    polled = latest;
}
