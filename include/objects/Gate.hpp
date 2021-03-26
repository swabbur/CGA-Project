#pragma once

#include <vector>
#include <glm/glm.hpp>
#include <graphics/Instance.hpp>

class Gate {
public:
    glm::vec3 position;
    Instance& part1, &part2, &part3, &part4;

    float animation_start;
    bool animated;

    Gate(glm::vec3 position, Instance &part1, Instance &part2, Instance &part3, Instance &part4);

    static std::vector<Gate> generate(Cache<std::string, struct Model> &, std::vector<Instance> &, glm::vec3 position);

    void animate(float time);
};
