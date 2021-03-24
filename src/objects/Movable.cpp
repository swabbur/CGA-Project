#include <objects/Movable.hpp>
#include <cmath>

Movable::Movable(Instance& instance, glm::vec2 position) : position(position), direction(glm::vec2(0.0f, 1.0f)) {
	instances.push_back(&instance);
}

Movable::Movable(std::vector<Instance*> instances, glm::vec2 position) : instances(instances), position(position){
	for (int i = 1; i < this->instances.size(); i++) {
		this->instances[i]->visible = false;
	}
}

void Movable::move(glm::vec2 position_delta) {
	position += position_delta;
	instances[active_instance]->position.x = position.x;
	instances[active_instance]->position.z = position.y;
}

void Movable::set_direction(glm::vec2 direction) {
	this->direction = glm::normalize(direction);
	float rotation = atan2(this->direction.x, this->direction.y);
	instances[active_instance]->rotation.y = rotation;
}

void Movable::update_active_instance(int index) {
	if (active_instance != index) {
		instances[active_instance]->visible = false;
		active_instance = index;
		instances[active_instance]->visible = true;
		move(glm::vec2(0.0f));
		set_direction(direction);
	}
}

Instance& Movable::get_instance() {
	return *instances[active_instance];
}

glm::vec2 Movable::get_position() {
	return position;
}

glm::vec2 Movable::get_direction() {
    return direction;
}
