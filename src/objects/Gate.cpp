#include <objects/Gate.hpp>
#include <glm/gtc/constants.hpp>
#include <iostream>

void Gate::generate(std::vector<Instance> &instances, Cache<std::string, struct Model> &cache) {
    for (int i = 0; i < 2; i++) {
        std::cout << instances.size() << std::endl;
        instances.push_back(Instance::create_static(cache.get("models/gate-part1.fbx")));
        Instance& part1 = instances.back();
        instances.push_back(Instance::create_static(cache.get("models/gate-part2.fbx"), part1));
        Instance& part2 = instances.back();

        instances.push_back(Instance::create_static(cache.get("models/gate-part3.fbx"), part1));
        Instance& part3 = instances.back();
        instances.push_back(Instance::create_static(cache.get("models/gate-part4.fbx"), part2));
        Instance& part4 = instances.back();
        std::cout << instances.size() << std::endl;

        if (i == 1) {
            part1.position = glm::vec3(0, 2, 0);
            part1.rotation[0] = glm::pi<float>();
        }

        part2.position = glm::vec3(0, 1, 0);
        part3.position = glm::vec3(-0.5f, 1, 0);
        part4.position = glm::vec3(-0.5f, 0, 0);
    }
}
