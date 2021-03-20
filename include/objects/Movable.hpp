#pragma once

#include <glm/glm.hpp>
#include "../graphics/Entity.hpp"

class Movable {

	glm::vec2 position;

public:

	Entity& entity;

	Movable(Entity & entity, glm::vec2 position);

	void move(glm::vec2 position_delta);

	glm::vec2 get_position();
};