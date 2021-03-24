#pragma once

#include "../graphics/Instance.hpp"

#include <glm/glm.hpp>

class Movable {

	glm::vec2 position;
	glm::vec2 direction;

	std::vector<Instance*> instances;

	int active_instance = 0;

public:

	Movable(Instance& instance, glm::vec2 position);

	Movable(std::vector<Instance*> instances, glm::vec2 position);

	void move(glm::vec2 position_delta);

	void set_direction(glm::vec2 direction);

	void update_active_instance(int index);

	Instance& get_instance();

	glm::vec2 get_position();
};