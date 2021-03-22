#pragma once

#include <glm/glm.hpp>
#include "Movable.hpp"

class Player {

public:

	Movable movable;

	Player(Instance& instance, glm::vec2 position);

	Player(std::vector<Instance*> instances, glm::vec2 position);
};

