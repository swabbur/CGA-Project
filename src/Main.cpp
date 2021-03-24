#include <devices/DeviceManager.hpp>
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <graphics/lights/DirectionalLight.hpp>
#include <graphics/lights/SpotLight.hpp>
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
#include <physics/Collision.hpp>

// Replace this include using Key/Button enum classes
#include <GLFW/glfw3.h>
#include <objects/Player.hpp>
#include <iostream>
#include <glm/gtc/matrix_inverse.hpp>

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
    camera.place(glm::vec3(1.5, 0.75, 0.5));

    Cache<std::string, Model> models(Model::load);
    std::vector<Instance> instances;
    instances.emplace_back("models/scene.fbx", models);
    instances.emplace_back("models/player/Human_standing.fbx", models);
    instances.emplace_back("models/player/Human_walking_", 1, 31, ".fbx", models);

    instances[1].position = glm::vec3(-0.2f, 0.1f, -0.4f);
    instances[2].position = glm::vec3(-0.2f, 0.1f, -0.4f);

    Timer timer;

    DirectionalLight directional_light;
    directional_light.color = glm::vec3(1.0f);
    directional_light.direction = glm::normalize(glm::vec3(1.0f, 2.0f, 3.0f));
    directional_light.intensity = 2.0f;

    SpotLight spot_light;
    spot_light.color = glm::vec3(1.0f);
    spot_light.position = glm::vec3(0.25f, 2.0f, 0.0f);
    spot_light.direction = glm::vec3(0.0f, -1.0f, 0.0f);
    spot_light.angle = glm::quarter_pi<float>() / 2.0; // Angle from center vector
    spot_light.intensity = 5.0f;

    Program shadow_program = Program::load({ "shaders/shadow_vertex.glsl", "shaders/debug_fragment.glsl" });
    ShadowMap shadow_map_1 = ShadowMap::create(2048);
    ShadowMap shadow_map_2 = ShadowMap::create(2048);

    std::vector<Instance*> player_instances = { &instances[1],  &instances[2] };
    Player player(player_instances, glm::vec2(-0.2f, -0.4f));
    float walk_speed = 0.3f;

    bool move_player = false;

    while (!window.is_closed()) {

        // Poll events
        keyboard.poll();
        mouse.poll();
        window.poll();

        // Exit on ESC press
        if (keyboard.is_pressed(GLFW_KEY_ESCAPE)) {
            break;
        }

        // Toggle camera/player movement
        if (keyboard.is_pressed(GLFW_KEY_C)) {
            move_player = !move_player;
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
            glm::vec3 normalized = timer.get_delta() * glm::normalize(direction);

            // Check collision
            if (move_player) {
                normalized *= walk_speed;
                player.movable.update_active_instance(1);
                for (int i = 0; i < instances[0].get_model(0).shapes.size(); i++) {// Shape const& shape : entities[0].scene.shapes) {
                    if (i == 0) { continue; } // We don't want to collide with the floor
                    Shape const& shape = instances[0].get_model(0).shapes[i];
                    glm::vec2 collided_direction = Collision::resolve_collision(player.movable.get_instance().get_model(0).shapes[0], shape, player.movable.get_position(), glm::vec2(0.0f), glm::vec2(normalized.x, normalized.z));
                    normalized.x = collided_direction.x;
                    normalized.z = collided_direction.y;
                }
                camera.move_orthogonal(normalized);
                player.movable.move(glm::vec2(normalized.x, normalized.z));
                player.movable.set_direction(glm::vec2(direction.x, direction.z));
            }
            else {
                camera.move(normalized);
            }
        }
        else if (move_player) {
            player.movable.update_active_instance(0);
        }

        if (mouse.is_down(GLFW_MOUSE_BUTTON_LEFT)) {
            float scale = std::min(window.get_width(), window.get_height()) / 2.0f;
            camera.rotate(glm::vec2(mouse.get_dy(), mouse.get_dx()) / scale);
        }

        // Update light
        if (keyboard.is_down(GLFW_KEY_UP)) {
            spot_light.position.y += timer.get_delta();
        }

        if (keyboard.is_down(GLFW_KEY_DOWN)) {
            spot_light.position.y -= timer.get_delta();
        }

        if (keyboard.is_pressed(GLFW_KEY_L)) {
            glm::mat4 rotation_matrix = camera.get_rotation_matrix();
            glm::vec3 camera_direction = rotation_matrix * glm::vec4(0.0f, 0.0f, -1.0f, 1.0f);
            spot_light.position = camera.get_position();
            spot_light.direction = camera_direction;
        }

        // Render shadow maps

        int animation_frame = timer.get_time() * 30; // Animation plays at 30 frames per second

        // Render shadow map
        {
            // Set context options
            context.set_multisampling(false);

            // Render directional light's shadow map
            {
                // Prepare framebuffer
                shadow_map_1.framebuffer.bind();
                context.set_view_port(0, 0, shadow_map_1.size, shadow_map_1.size);
                context.clear();

                // Render instances
                shadow_program.bind();
                for (Instance & instance : instances) {
                    if (instance.visible) {

                        // Set MVP matrix
                        glm::mat4 light_mvp = directional_light.get_projection_matrix(5.0f, 5.0f, -5.0f, 10.0f)
                            * directional_light.get_view_matrix()
                            * instance.get_transformation();
                        shadow_program.set_mat4("mvp", light_mvp);

                        // Render shapes
                        for (Shape const& shape : instance.get_model(animation_frame).shapes) {
                            shape.mesh.draw();
                        }
                    }
                }
            }

            // Render spot light's shadow map
            {
                // Prepare framebuffer
                shadow_map_2.framebuffer.bind();
                context.set_view_port(0, 0, shadow_map_2.size, shadow_map_2.size);
                context.clear();

                // Render instances
                shadow_program.bind();
                for (Instance & instance : instances) {
                    if (instance.visible) {

                        // Set MVP matrix
                        glm::mat4 light_mvp = spot_light.get_projection(0.1f, 10.0f)
                            * spot_light.get_view()
                            * instance.get_transformation();
                        shadow_program.set_mat4("mvp", light_mvp);

                        // Render shapes
                        for (Shape const& shape : instance.get_model(animation_frame).shapes) {
                            shape.mesh.draw();
                        }
                    }
                }
            }
        }

        // Render scene
        {
            // Set context options
            context.set_multisampling(true);

            // Prepare framebuffer
            framebuffer.bind();
            context.set_view_port(0, 0, window.get_width(), window.get_height());
            context.clear();

            // Bind shader program
            program.bind();

            // Set camera properties
            program.set_vec3("camera.position", camera.get_position());

            // Set light properties
            program.set_vec3("directional_light.color", directional_light.color);
            program.set_vec3("directional_light.direction", directional_light.direction);
            program.set_float("directional_light.intensity", directional_light.intensity);
            shadow_map_1.texture.bind(4);
            program.set_sampler("directional_light.shadow_sampler", 4);
            {
                glm::mat4 light_vp = directional_light.get_projection_matrix(5.0f, 5.0f, -5.0f, 10.0f)
                                     * directional_light.get_view_matrix();
                glm::mat4 sampling_adjustment = glm::scale(glm::translate(glm::vec3(0.5f)), glm::vec3(0.5f));
                glm::mat4 adjusted_light_vp = sampling_adjustment * light_vp;
                program.set_mat4("directional_light.vp", adjusted_light_vp);
            }

            program.set_vec3("spot_light.color", spot_light.color);
            program.set_vec3("spot_light.position", spot_light.position);
            program.set_vec3("spot_light.direction", spot_light.direction);
            program.set_float("spot_light.angle", spot_light.angle);
            program.set_float("spot_light.intensity", spot_light.intensity);
            shadow_map_2.texture.bind(5);
            program.set_sampler("spot_light.shadow_sampler", 5);
            {
                glm::mat4 light_vp = spot_light.get_projection(0.1f, 10.0f)
                                     * spot_light.get_view();
                program.set_mat4("spot_light.vp", light_vp);
            }

            // Render instances
            for (Instance & instance : instances) {
                if (instance.visible) {

                // Set transformation matrices
                glm::mat4 position_transformation = instance.get_transformation();
                glm::mat3 normal_transformation = glm::inverseTranspose(glm::mat3(position_transformation));
                program.set_mat4("position_transformation", position_transformation);
                program.set_mat3("normal_transformation", normal_transformation);

                // Set camera MVP
                glm::mat4 mvp = camera.get_projection_matrix()
                                * camera.get_view_matrix()
                                * instance.get_transformation();
                program.set_mat4("mvp", mvp);

                // Render shapes
                for (Shape const & shape : instance.get_model(animation_frame).shapes) {

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
            }
        }

        // Update window
        window.swap_buffers();

        // Update timer
        timer.update();
    }

    return 0;
}
