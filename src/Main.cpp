#include <devices/DeviceManager.hpp>
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <graphics/lights/DirectionalLight.hpp>
#include <graphics/Context.hpp>
#include <graphics/Framebuffer.hpp>
#include <graphics/Instance.hpp>
#include <graphics/Model.hpp>
#include <graphics/Program.hpp>
#include <graphics/ShadowMap.hpp>
#include <graphics/Texture.hpp>
#include <util/Cache.hpp>
#include <util/Camera.hpp>
#include <util/Timer.hpp>

// Replace this include using Key/Button enum classes
#include <GLFW/glfw3.h>

int main() {

    DeviceManager device_manager;
    Window & window = device_manager.get_window();
    Keyboard & keyboard = device_manager.get_keyboard();
    Mouse & mouse = device_manager.get_mouse();

    Context context(window);
    context.set_clear_color(0.5f, 0.5f, 0.5f);
    context.set_clear_depth(1.0f);
    context.set_depth_test(true);
    context.set_alpha_blending(false);
    context.set_cull_face(true);

    Framebuffer framebuffer = Framebuffer::get_default();
    Program program = Program::load({ "shaders/vertex.glsl", "shaders/fragment.glsl" });
    Camera camera;
    camera.turn(glm::vec2(0.407407, -1.42222));
    camera.place(glm::vec3(1.53824, 0.761792, 0.354391));

    Cache<std::string, Model> models(Model::load);
    std::vector<Instance> instances;
    instances.emplace_back(models.get("models/scene.fbx"));

    Timer timer;

    DirectionalLight directional_light;
    directional_light.color = glm::vec3(1.0f);
    directional_light.direction = glm::normalize(glm::vec3(1.0f, 2.0f, 3.0f));
    directional_light.intensity = 4.0f;

    ShadowMap shadow_map = ShadowMap::create(2048);
    Program shadow_program = Program::load({ "shaders/shadow_vertex.glsl" });

    while (!window.is_closed()) {

        // Poll events
        keyboard.poll();
        mouse.poll();
        window.poll();

        // Exit on ESC press
        if (keyboard.is_pressed(GLFW_KEY_ESCAPE)) {
            break;
        }

        // Update camera
        camera.set_aspect_ratio(window.get_aspect_ratio());

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

        if (mouse.is_down(GLFW_MOUSE_BUTTON_LEFT)) {
            float scale = std::min(window.get_width(), window.get_height()) / 2.0f;
            camera.rotate(glm::vec2(mouse.get_dy(), mouse.get_dx()) / scale);
        }

        // Render shadow map
        {
            // Set context options
            context.set_multisampling(false);

            // Prepare framebuffer
            shadow_map.framebuffer.bind();
            context.set_view_port(0, 0, shadow_map.size, shadow_map.size);
            context.clear();

            // Render instances
            shadow_program.bind();
            for (Instance & instance : instances) {

                // Set MVP matrix
                glm::mat4 projection = glm::ortho(-5.0f, 5.0f, -5.0f, 5.0f, -5.0f, 10.0f);
                glm::mat4 view = glm::lookAt(directional_light.direction, glm::vec3(), glm::vec3(0.0f, 1.0f, 0.0f));
                glm::mat4 mvp = projection * view * instance.get_transformation();
                shadow_program.set_mat4("mvp", mvp);

                // Render shapes
                for (Shape const & shape : instance.model.shapes) {
                    shape.mesh.draw();
                }
            }
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

        // Set camera properties
        program.set_vec3("camera.position", camera.get_position());

        // Set light properties
        program.set_vec3("directional_light.color", directional_light.color);
        program.set_vec3("directional_light.direction", directional_light.direction);
        program.set_float("directional_light.intensity", directional_light.intensity);

        // Render instances
        for (Instance & instance : instances) {

            // Set MVP matrices
            glm::mat4 mvp = camera.get_projection_matrix() * camera.get_view_matrix() * instance.get_transformation();
            program.set_mat4("mvp", mvp);
            program.set_mat4("position_transformation", instance.get_transformation());
            program.set_mat3("normal_transformation",
                             glm::transpose(glm::inverse(glm::mat3(instance.get_transformation()))));

            // Set shadow map properties
            shadow_map.texture.bind(4);
            program.set_sampler("directional_light.shadow.sampler", 4);
            glm::mat4 shadow_mvp = glm::ortho(-5.0f, 5.0f, -5.0f, 5.0f, -5.0f, 10.0f)
                                   * glm::lookAt(directional_light.direction, glm::vec3(), glm::vec3(0.0f, 1.0f, 0.0f))
                                   * instance.get_transformation();
            glm::mat4 bias(0.5, 0.0, 0.0, 0.0,
                           0.0, 0.5, 0.0, 0.0,
                           0.0, 0.0, 0.5, 0.0,
                           0.5, 0.5, 0.5, 1.0);
            glm::mat4 biased_shadow_mvp = bias * shadow_mvp;
            program.set_mat4("directional_light.shadow.mvp", biased_shadow_mvp);
            program.set_int("directional_light.shadow.size", shadow_map.size);

            // Render shapes
            for (Shape const & shape : instance.model.shapes) {

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
