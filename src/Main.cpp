#include <devices/DeviceManager.hpp>
#include <graphics/lights/PointLight.hpp>
#include <graphics/lights/SpotLight.hpp>
#include <graphics/Context.hpp>
#include <graphics/Framebuffer.hpp>
#include <graphics/Model.hpp>
#include <graphics/Program.hpp>
#include <graphics/Texture.hpp>
#include <util/Camera.hpp>
#include <util/Timer.hpp>
#include <graphics/Entity.hpp>

// Replace this include using Key/Button enum classes
#include <GLFW/glfw3.h>

int main() {

    DeviceManager device_manager;
    Window & window = device_manager.get_window();
    Keyboard & keyboard = device_manager.get_keyboard();
    Mouse & mouse = device_manager.get_mouse();

    Context context(window);
    Framebuffer framebuffer = Framebuffer::get_default();
    Program program = Program::load({ "shaders/vertex.glsl", "shaders/fragment.glsl" });

    std::vector<Model> scenes;
    scenes.push_back(Model::load("scenes/scene.dae"));

    std::vector<Entity> entities;
    entities.emplace_back(scenes[0]);

    Texture shadow_map = Texture::create(Texture::Type::DEPTH, 1024, 1024);
    Framebuffer shadow_framebuffer = Framebuffer::create({ shadow_map });
    Program shadow_program = Program::load({ "shaders/shadow_vertex.glsl" });

    Camera camera(window, glm::vec3(0.0f, 0.75f, 1.5f), glm::vec2(0.0f, 0.0f));
    Timer timer;

    PointLight point_light;
    point_light.ambient = glm::vec3(0.0f);
    point_light.diffuse = glm::vec3(1000.0f);
    point_light.specular = glm::vec3(1000.0f);
    point_light.position = glm::vec3(2.0f, 2.5f, -1.0f);

    SpotLight spot_light;

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
        context.set_multisampling(true);
        context.set_depth_test(true);
        context.set_cull_face(true);
        context.set_alpha_blending(false);

        // Bind shader program
        program.bind();

        // Set camera position
        program.set_vec3("camera.position", camera.get_position());

        // Set light properties
        program.set_vec3("point_light.ambient", point_light.ambient);
        program.set_vec3("point_light.diffuse", point_light.diffuse);
        program.set_vec3("point_light.specular", point_light.specular);
        program.set_vec3("point_light.position", point_light.position);

        program.set_vec3("spot_light.ambient", spot_light.ambient);
        program.set_vec3("spot_light.diffuse", spot_light.diffuse);
        program.set_vec3("spot_light.specular", spot_light.specular);
        program.set_vec3("spot_light.position", spot_light.position);
        program.set_vec3("spot_light.direction", spot_light.direction);
        program.set_float("spot_light.inner_angle", spot_light.angles.inner);
        program.set_float("spot_light.outer_angle", spot_light.angles.outer);

        // Render entities
        for (Entity & entity : entities) {

            // Set MVP matrix
            glm::mat4 mvp = camera.get_projection_matrix() * camera.get_view_matrix() * entity.get_model_matrix();
            program.set_mat4("mvp", mvp);
            program.set_mat4("position_transformation", entity.get_model_matrix());
            program.set_mat3("normal_transformation", glm::transpose(glm::inverse(glm::mat3(entity.get_model_matrix()))));

            // Render shapes
            for (Shape const& shape : entity.scene.shapes) {

                // Set material properties
                if (auto texture = std::get_if<Texture>(&shape.material.ambient)) {
                    texture->bind(0);
                    program.set_bool("material.ambient.textured", true);
                    program.set_sampler("material.ambient.sampler", 0);
                } else if (auto color = std::get_if<glm::vec3>(&shape.material.ambient)) {
                    program.set_bool("material.ambient.textured", false);
                    program.set_vec3("material.ambient.color", *color);
                }

                if (auto texture = std::get_if<Texture>(&shape.material.diffuse)) {
                    texture->bind(1);
                    program.set_bool("material.diffuse.textured", true);
                    program.set_sampler("material.diffuse.sampler", 1);
                } else if (auto color = std::get_if<glm::vec3>(&shape.material.diffuse)) {
                    program.set_bool("material.diffuse.textured", false);
                    program.set_vec3("material.diffuse.color", *color);
                }

                if (auto texture = std::get_if<Texture>(&shape.material.specular)) {
                    texture->bind(2);
                    program.set_bool("material.specular.textured", true);
                    program.set_sampler("material.specular.sampler", 2);
                } else if (auto color = std::get_if<glm::vec3>(&shape.material.specular)) {
                    program.set_bool("material.specular.textured", false);
                    program.set_vec3("material.specular.color", *color);
                }

                if (auto shininess = std::get_if<float>(&shape.material.shininess)) {
                    program.set_float("material.shininess", *shininess);
                }

                if (auto texture = std::get_if<Texture>(&shape.material.normal)) {
                    texture->bind(3);
                    program.set_bool("material.normal_textured", true);
                    program.set_sampler("material.normal_sampler", 3);
                } else {
                    program.set_bool("material.normal_textured", false);
                }

                // Render mesh
                shape.mesh.draw();
            }
        }

        // Update window
        window.swap_buffers();

        // Update timer
        timer.update();
    }

    return 0;
}
