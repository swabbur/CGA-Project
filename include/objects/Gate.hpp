#pragma once

#include <vector>
#include <glm/glm.hpp>
#include <graphics/Instance.hpp>

class Gate {
public:
    static void generate(std::vector<Instance> &, Cache<std::string, struct Model> &);
};
