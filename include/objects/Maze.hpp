#pragma once

#include <vector>
#include <glm/glm.hpp>
#include <graphics/Instance.hpp>

class Maze {
public:
    static void generate(std::vector<Instance> &, Cache<std::string, struct Model> &);
};
