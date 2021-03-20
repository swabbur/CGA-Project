#include <objects\Movable.hpp>

Movable::Movable(Instance& instance, glm::vec2 position) : instance(instance), position(position) {}

void Movable::move(glm::vec2 position_delta) {
	position += position_delta;
	instance.position.x = position.x;
	instance.position.z = position.y;
}

glm::vec2 Movable::get_position() {
	return position;
}
