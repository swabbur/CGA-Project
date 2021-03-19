#pragma once

#include <glm/glm.hpp>
#include "../graphics/Entity.hpp"

class Movable {

	glm::vec2 position;
	Entity& entity;

public:

	Movable(Entity & entity, glm::vec2 position);

	void move(glm::vec2 position_delta);
};