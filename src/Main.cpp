#include <devices/DeviceManager.hpp>
#include <graphics/Context.hpp>
#include <graphics/Framebuffer.hpp>
#include <graphics/Scene.hpp>
#include <graphics/Program.hpp>
#include <graphics/Texture.hpp>
#include <util/Camera.hpp>
#include <util/Timer.hpp>

// Replace this include using Key/Button enum classes
#include <GLFW/glfw3.h>

#include <glm/gtx/transform.hpp>

int main() {

    DeviceManager device_manager;
    Window & window = device_manager.get_window();
    Keyboard & keyboard = device_manager.get_keyboard();
    Mouse & mouse = device_manager.get_mouse();

    Context context(window);
    Framebuffer framebuffer = Framebuffer::get_default();
    Program program = Program::load({ "shaders/vertex.glsl", "shaders/fragment.glsl" });
    Scene const & scene = Scene::load("scenes/scene.dae");

    Camera camera(window, glm::vec3(0.0f, 0.75f, 1.5f), glm::vec2(0.0f, 0.0f));
    Timer timer;

    while (!window.is_closed()) {

        // Poll events
        keyboard.poll();
        mouse.poll();
        window.poll();

        // Exit on ESC
        if (keyboard.is_pressed(GLFW_KEY_ESCAPE)) {
            break;
        }

        // Update camera position based on keyboard input
        glm::vec3 direction(0.0f);

        if (keyboard.is_down(GLFW_KEY_W)) {
            direction.z -= 1.0f;
        }

        if (keyboard.is_down(GLFW_KEY_S)) {
            direction.z += 1.0f;
        }

        if (keyboard.is_down(GLFW_KEY_A)) {
            direction.x -= 1.0f;
        }

        if (keyboard.is_down(GLFW_KEY_D)) {
            direction.x += 1.0f;
        }

        if (keyboard.is_down(GLFW_KEY_SPACE)) {
            direction.y += 1.0f;
        }

        if (keyboard.is_down(GLFW_KEY_LEFT_SHIFT)) {
            direction.y -= 1.0f;
        }

        if (glm::dot(direction, direction) != 0.0f) {
            camera.move(timer.get_delta() * glm::normalize(direction));
        }

        // Update camera rotation based on mouse input
        if (mouse.is_down(GLFW_MOUSE_BUTTON_LEFT)) {
            float scale = std::min(window.get_width(), window.get_height()) / 2.0f;
            camera.rotate(glm::vec2(mouse.get_dy(), mouse.get_dx()) / scale);
        }

        // Bind framebuffer
        framebuffer.bind();

        // Clear framebuffer
        context.set_view_port(0, 0, window.get_width(), window.get_height());
        context.set_clear_color(0.5f, 0.5f, 0.5f);
        context.set_clear_depth(1.0f);
        context.clear();

        // Set additional context options
        context.set_depth_test(true);
        context.set_cull_face(false);
        context.set_alpha_blending(true);

        // Bind shader program
        program.bind();

        // Set MVP matrix
        glm::mat4 mvp = camera.get_projection_matrix() * camera.get_view_matrix();
        program.set_matrix(0, mvp);

        // Render scene
        for (Shape const & shape : scene.shapes) {

            // Bind texture
            if (auto diffuse_texture = std::get_if<Texture>(&shape.material.diffuse)) {
                diffuse_texture->bind(0);
                program.set_texture(2, 0);
            }

            // Draw mesh
            shape.mesh.draw();
        }

        // Update window
        window.swap_buffers();

        // Update timer
        timer.update();
    }

    return 0;
}
