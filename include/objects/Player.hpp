#pragma once

#include <glm/glm.hpp>
#include "Movable.hpp"

class Player {

public:

	Movable movable;

	Player(Entity & entity, glm::vec2 position);
};

