#pragma once

#include <vector>
#include <glm/glm.hpp>
#include <graphics/Instance.hpp>

class Maze {
public:
    static void generate(Cache<std::string, struct Model> &, std::vector<Instance> &);
};
