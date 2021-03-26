#include <objects/Gate.hpp>
#include <glm/gtc/constants.hpp>
#include <iostream>

Gate::Gate(glm::vec3 position, Instance &part1, Instance &part2, Instance &part3, Instance &part4) : part1(part1), part2(part2), part3(part3),
    part4(part4), animated(false), animation_start(0), position(position) {}

std::vector<Gate> Gate::generate(Cache<std::string, struct Model> &cache, std::vector<Instance> &instances, glm::vec3 position) {
    std::vector<Gate> result;

    for (int i = 0; i < 2; i++) {
        instances.push_back(Instance::create_static(cache.get("models/gate-part1.fbx")));
        Instance& part1 = instances.back();
        instances.push_back(Instance::create_static(cache.get("models/gate-part2.fbx"), part1));
        Instance& part2 = instances.back();
        instances.push_back(Instance::create_static(cache.get("models/gate-part3.fbx"), part1));
        Instance& part3 = instances.back();
        instances.push_back(Instance::create_static(cache.get("models/gate-part4.fbx"), part2));
        Instance& part4 = instances.back();

        part1.position = position + glm::vec3((2 * i - 1) * 0.5f, 2 * i, 0);
        part1.rotation = glm::vec2(glm::pi<float>() * i, (2 * i - 1) * glm::half_pi<float>());
        part2.position = glm::vec3(0, 1, -.5f);
        part3.position = glm::vec3(0, 1, 0);
        part4.position = glm::vec3(0, 0, .5f);

        result.emplace_back(position, part1, part2, part3, part4);
    }

    return result;
}

void Gate::animate(float time) {
    if (!animated) return;
    float delta = time - animation_start;

    part4.rotation[0] = part3.rotation[0] = glm::max(glm::min(delta, 2.0f), 0.0f) / 4 * -glm::pi<float>();
    part4.position[0] = glm::max(glm::min(delta - 2.0f, 1.0f), 0.0f) * -0.025;
    part2.rotation[0] = glm::max(glm::min(delta - 3.0f, 2.0f), 0.0f) * glm::pi<float>() / 4;
    part2.position[0] = glm::max(glm::min(delta - 5.0f, 1.0f), 0.0f) * -0.025;
    part1.position = position + glm::vec3(
            -glm::cos(part1.rotation[0]) * (0.5f + glm::max(glm::min(delta - 6.0f, 2.0f), 0.0f) / 2),
            1 - glm::cos(part1.rotation[0]),
            0);
}
