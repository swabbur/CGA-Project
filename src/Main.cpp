#include <devices/DeviceManager.hpp>
#include <devices/Gamepad.hpp>
#include <glm/gtc/matrix_inverse.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/glm.hpp>
#include <graphics/lights/DirectionalLight.hpp>
#include <graphics/lights/SpotLight.hpp>
#include <graphics/Context.hpp>
#include <graphics/Framebuffer.hpp>
#include <graphics/Instance.hpp>
#include <graphics/Model.hpp>
#include <graphics/Program.hpp>
#include <graphics/ShadowMap.hpp>
#include <graphics/Texture.hpp>
#include <objects/Maze.hpp>
#include <objects/Player.hpp>
#include <physics/Collision.hpp>
#include <util/Cache.hpp>
#include <util/Camera.hpp>
#include <util/Timer.hpp>

// Replace this include using Key/Button enum classes
#include <GLFW/glfw3.h>
#include <iostream>

int main() {

    DeviceManager device_manager;
    Window & window = device_manager.get_window();
    Keyboard & keyboard = device_manager.get_keyboard();
    Mouse & mouse = device_manager.get_mouse();
    Gamepad gamepad(0);

    Context context(window);
    context.set_clear_color(0.5f, 0.5f, 0.5f);
    context.set_clear_depth(1.0f);
    context.set_depth_test(true);
    context.set_alpha_blending(false);
    context.set_cull_face(true);

    Framebuffer framebuffer = Framebuffer::get_default();
    Program program = Program::load({ "shaders/vertex.glsl", "shaders/fragment.glsl" });
    Camera camera(glm::vec3(), 4.0f * glm::vec3(0.0f, 1.0f, 1.0f));

    DirectionalLight directional_light;
    directional_light.color = glm::vec3(1.0f);
    directional_light.direction = glm::normalize(glm::vec3(1.0f, 2.0f, 3.0f));
    directional_light.intensity = 2.0f;

    SpotLight spot_light;
    spot_light.color = glm::vec3(1.0f);
    spot_light.position = glm::vec3(1.0f, 8.0f, 0.0f);
    spot_light.direction = glm::vec3(0.0f, -1.0f, 0.0f);
    spot_light.angle = glm::quarter_pi<float>() / 2.0; // Angle from center vector
    spot_light.intensity = 12.0f;

    Program shadow_program = Program::load({ "shaders/shadow_vertex.glsl" });
    ShadowMap shadow_map_1 = ShadowMap::create(4096);
    ShadowMap shadow_map_2 = ShadowMap::create(2048);
    ShadowMap shadow_map_3 = ShadowMap::create(2048);

    Cache<std::string, Model> models(Model::load);

    std::vector<Instance> instances;
    instances.emplace_back(models.get("models/scene.fbx"));
    instances.emplace_back(models.get("models/player/Human_standing.fbx"));
    instances.push_back(Instance::create(models, "models/player/Human_walking_", 1, 31, ".fbx"));
    instances.emplace_back(models.get("models/key.fbx"));
    instances.emplace_back(models.get("models/pedestal.dae"));
    Maze::generate(instances, models);

    Instance & key = instances[3];
    key.position = glm::vec3(2.5f, 1.0f, 3.0f);
    Instance & pedestal = instances[4];
    pedestal.position = glm::vec3(2.5f, 0.0f, 3.0f);

    Player player({ instances[1], instances[2] }, glm::vec2(-0.2f, -0.4f), glm::vec2(0.0f, -1.0f), 1.2f);

    std::set<int> collision_exceptions = {1, 2};

    Texture toon_map = Texture::load("textures/toon_map.png");

    Timer timer;
    float animation_progress = 0.0f;

    while (!window.is_closed()) {

        // Poll events
        keyboard.poll();
        mouse.poll();
        window.poll();
        gamepad.poll();

        // Exit on ESC press
        if (keyboard.is_pressed(GLFW_KEY_ESCAPE)) {
            break;
        }

        // Animate key
        float offset = 0.075f * glm::cos(glm::half_pi<float>() * timer.get_time());
        key.position = glm::vec3(2.5f, 1.0f + offset, 3.0f);
        key.rotation = glm::vec2(0.0f, glm::half_pi<float>() * timer.get_time());

        // Pick-up key
        glm::vec2 key_position = glm::vec2(key.position.x, key.position.z);
        if (glm::distance(key_position, player.get_position()) < 0.75f) {
            if (keyboard.is_pressed(GLFW_KEY_SPACE) || gamepad.is_pressed(GLFW_GAMEPAD_BUTTON_A)) {
                key.visible = !key.visible;
            }
        }

        // Update player
        glm::vec2 direction(0.0f);

        if (keyboard.is_down(GLFW_KEY_W)) {
            direction.y -= 1.0f;
        }

        if (keyboard.is_down(GLFW_KEY_S)) {
            direction.y += 1.0f;
        }

        if (keyboard.is_down(GLFW_KEY_A)) {
            direction.x -= 1.0f;
        }

        if (keyboard.is_down(GLFW_KEY_D)) {
            direction.x += 1.0f;
        }

        // Normalize keyboard-based direction
        if (glm::length(direction) != 0.0f) {
            direction = glm::normalize(direction);
        }

        if (gamepad.is_connected()) {
            direction.x += gamepad.get_axis(GLFW_GAMEPAD_AXIS_LEFT_X);
            direction.y += gamepad.get_axis(GLFW_GAMEPAD_AXIS_LEFT_Y);
        }

        if (glm::length(direction) > 0.1f) {

            // Update walking animation
            animation_progress += glm::length(direction) * timer.get_delta();

            // Compute translation
            glm::vec2 translation = direction * timer.get_delta() * player.get_speed();

            // Check for collisions
            for (int j = 0; j < instances.size(); j++) {
                if (collision_exceptions.contains(j)) continue;
                Instance* instance = &instances[j];
                for (Shape const & shape : instance->get_model(0).shapes) {
                    // Adjust for collision
                    Shape const &colliding_shape = player.get_instance().get_model(0).shapes[0];
                    translation = Collision::resolve_collision(colliding_shape,
                                                               shape,
                                                               player.get_position(),
                                                               glm::vec2(instance->position[0], instance->position[2]),
                                                               player.get_direction(),
                                                               glm::vec2(cos(instance->rotation[1]), sin(instance->rotation[1])),
                                                               translation);
                }
            }

            // Update the player
            player.activate_instance(1);
            player.move(translation);
            player.turn(direction);

        } else {

            // Update the player
            player.activate_instance(0);
        }

        // Activate toon shading
        bool toon_shading_active;
        if (mouse.is_down(1)) {
            toon_shading_active = true;
        }
        else {
            toon_shading_active = false;
        }

        // Update camera
        glm::vec2 player_position = player.get_position();
        glm::vec2 player_direction = player.get_direction();
        camera.focus(glm::vec3(player_position.x, 0.0f, player_position.y));
        camera.set_aspect_ratio(window.get_aspect_ratio());

        // Update light
        spot_light.direction = glm::normalize(glm::vec3(player_direction.x, -0.25f, player_direction.y));
        spot_light.position = glm::vec3(player_position.x, 0.8f, player_position.y) + spot_light.direction * 0.1f;

        // Select animation frame
        // Animation plays at 30 frames per second
        int animation_frame = glm::floor(30 * animation_progress);

        // Render shadow maps
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
                        glm::vec2 position = player.get_position();
                        glm::mat4 light_mvp = directional_light.get_projection_matrix(20.0f, 10.0f, -10.0f, 30.0f)
                            * directional_light.get_view_matrix(glm::vec3(position.x, 0.0f, position.y))
                            * instance.get_transformation();
                        shadow_program.set_mat4("mvp", light_mvp);

                        // Render shapes
                        for (Shape const & shape : instance.get_model(animation_frame).shapes) {
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
                        glm::mat4 light_mvp = spot_light.get_projection_matrix(0.1f, 10.0f)
                            * spot_light.get_view_matrix()
                            * instance.get_transformation();
                        shadow_program.set_mat4("mvp", light_mvp);

                        // Render shapes
                        for (Shape const & shape : instance.get_model(animation_frame).shapes) {
                            shape.mesh.draw();
                        }
                    }
                }
            }

            // Render x-ray light's shadow map
            {
                // Prepare framebuffer
                shadow_map_3.framebuffer.bind();
                context.set_view_port(0, 0, shadow_map_3.size, shadow_map_3.size);
                context.clear();

                // Render instances
                shadow_program.bind();
                for (Instance& instance : instances) {
                    if (instance.visible && instance.xrayable) {

                        // Set MVP matrix
                        glm::mat4 light_mvp = camera.get_projection_matrix()
                            * camera.get_view_matrix()
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
                glm::vec2 position = player.get_position();
                glm::mat4 light_vp = directional_light.get_projection_matrix(20.0f, 10.0f, -10.0f, 30.0f)
                                     * directional_light.get_view_matrix(glm::vec3(position.x, 0.0f, position.y));
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
                glm::mat4 light_vp = spot_light.get_projection_matrix(0.1f, 10.0f)
                                     * spot_light.get_view_matrix();
                program.set_mat4("spot_light.vp", light_vp);
            }

            program.set_vec3("xray_light.position", camera.get_position());
            {
                glm::mat4 light_vp = camera.get_projection_matrix()
                    * camera.get_view_matrix();
                glm::vec4 mouse_position(mouse.get_x()/window.get_width()*2.0f-1.0f, 1.0f-mouse.get_y()/window.get_height()*2.0f, 1.0, 1.0);
                glm::mat4 inverse_vp = glm::inverse(light_vp);
                glm::vec4 mouse_world_position = inverse_vp * mouse_position;

                program.set_vec3("xray_light.direction", glm::vec3(mouse_world_position) / mouse_world_position.w - camera.get_position());
                program.set_float("xray_light.angle", spot_light.angle/4.0f);
                shadow_map_3.texture.bind(6);
                program.set_sampler("xray_light.shadow_sampler", 6);
                program.set_mat4("xray_light.vp", light_vp);
            }

            // Set xray properties
            toon_map.bind(7);
            program.set_sampler("toon_map", 7);

            // Render instances
            for (Instance & instance : instances) {
                if (instance.visible) {

                    // Set x-ray variables per instance
                    program.set_bool("xrayable", (instance.xrayable & toon_shading_active));

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
