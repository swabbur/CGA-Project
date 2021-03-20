#include <devices/DeviceManager.hpp>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <mutex>
#include <stdexcept>
#include <future>
#include <iostream>

std::mutex mutex;
DeviceManager * device_manager = nullptr;

DeviceManager::DeviceManager() : handle(nullptr), keyboard(), mouse(), window(), running(false), thread() {

    std::unique_lock<std::mutex> lock(mutex);
    if (device_manager) {
        throw std::runtime_error("A device manager already exists");
    }
    device_manager = this;
    lock.unlock();

    // Initialize GLFW
    int glfw_initialized = glfwInit();
    if (glfw_initialized != GLFW_TRUE) {
        throw std::runtime_error("Could not initialize GLFW");
    }

    // Create event-loop
    std::promise<std::string> promise;

    running = true;
    thread = std::thread([this, &promise]() {

        // Set default window hints
        glfwDefaultWindowHints();

        // Set window look-and-feel
        glfwWindowHint(GLFW_DECORATED, GLFW_TRUE);
        glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);
        glfwWindowHint(GLFW_VISIBLE, GLFW_TRUE);

        // Set OpenGL preferences
        glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_API);

        // Set MSAA preferences
        glfwWindowHint(GLFW_SAMPLES, 4);

        // Compute initial width and height
        GLFWmonitor * monitor = glfwGetPrimaryMonitor();
        GLFWvidmode const * video_mode = glfwGetVideoMode(monitor);
        int width = video_mode->width;
        int height = video_mode->height;
        if (16 * width >= 9 * height) {
            height /= 2;
            width = 16 * height / 9;
        } else {
            width /= 2;
            height = 9 * width / 16;
        }

        // Create window
        handle = glfwCreateWindow(width, height, "Legacy", nullptr, nullptr);
        if (handle == nullptr) {
            throw std::runtime_error("Could not create GLFW window");
        }
        window.resize(width, height);

        int x = (video_mode->width - width) / 2;
        int y = (video_mode->height - height) / 2;
        glfwSetWindowPos(static_cast<GLFWwindow *>(handle), x, y);

        // Pass handle to devices
        keyboard.set_handle(handle);
        mouse.set_handle(handle);
        window.set_handle(handle);

        // Configure callbacks
        glfwSetWindowCloseCallback(static_cast<GLFWwindow *>(handle), [](GLFWwindow * /* window */) {
            device_manager->window.close();
        });

        glfwSetWindowSizeCallback(static_cast<GLFWwindow *>(handle), [](GLFWwindow * /* window */, int width, int height) {
            device_manager->window.resize(width, height);
        });

        glfwSetKeyCallback(static_cast<GLFWwindow *>(handle), [](GLFWwindow * /* window */, int key, int /* scancode */, int action, int /* mods */) {
            switch (action) {

                case GLFW_PRESS:
                    device_manager->keyboard.press(key);
                    break;

                case GLFW_RELEASE:
                    device_manager->keyboard.release(key);
                    break;

                default:
                    break;
            }
        });

        glfwSetMouseButtonCallback(static_cast<GLFWwindow *>(handle), [](GLFWwindow * /* window */, int button, int action, int /* mods */) {
            switch (action) {

                case GLFW_PRESS:
                    device_manager->mouse.press(button);
                    break;

                case GLFW_RELEASE:
                    device_manager->mouse.release(button);
                    break;

                default:
                    break;
            }
        });

        glfwSetCursorPosCallback(static_cast<GLFWwindow *>(handle), [](GLFWwindow * /* window */, double x, double y) {
            device_manager->mouse.move(static_cast<float>(x), static_cast<float>(y));
        });

        // Enable raw mouse input
        if (glfwRawMouseMotionSupported()) {
            glfwSetInputMode(static_cast<GLFWwindow *>(handle), GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);
        }

        promise.set_value("");

        while (running) {
            glfwWaitEventsTimeout(0.1);
        }
        device_manager->window.close();
    });

    std::future<std::string> future_error_message = promise.get_future();
    std::string error_message = future_error_message.get();
    if (!error_message.empty()) {
        throw std::runtime_error(error_message);
    }

    // Poll each device once as initialization
    keyboard.poll();
    mouse.poll();
    window.poll();
}

DeviceManager::~DeviceManager() {

    // Close event-loop
    running = false;
    if (thread.joinable()) {
        thread.join();
    }

    // Disable callbacks
    glfwSetWindowCloseCallback(static_cast<GLFWwindow *>(handle), nullptr);
    glfwSetWindowSizeCallback(static_cast<GLFWwindow *>(handle), nullptr);

    // Destroy window
    glfwDestroyWindow(static_cast<GLFWwindow *>(handle));

    // Terminate GLFW
    glfwTerminate();

    std::unique_lock<std::mutex> lock(mutex);
    device_manager = nullptr;
    lock.unlock();
}

[[nodiscard]] Keyboard & DeviceManager::get_keyboard() {
    return keyboard;
}

[[nodiscard]] Mouse & DeviceManager::get_mouse() {
    return mouse;
}

[[nodiscard]] Window & DeviceManager::get_window() {
    return window;
}
