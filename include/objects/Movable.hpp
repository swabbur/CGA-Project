#pragma once

#include <glm/glm.hpp>
#include "../graphics/Instance.hpp"

class Movable {

	glm::vec2 position;

public:

	Instance& instance;

	Movable(Instance& instance, glm::vec2 position);

	void move(glm::vec2 position_delta);

	glm::vec2 get_position();
};