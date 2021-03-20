#include <objects\Movable.hpp>

Movable::Movable(Entity & entity, glm::vec2 position) : entity(entity), position(position) {}

void Movable::move(glm::vec2 position_delta) {
	position += position_delta;
	entity.position.x = position.x;
	entity.position.z = position.y;
}

glm::vec2 Movable::get_position() {
	return position;
}
