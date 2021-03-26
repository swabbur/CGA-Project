#include <objects/Player.hpp>
#include <cmath>

Player::Player(std::vector<std::reference_wrapper<Instance>> const & instances,
               glm::vec2 position,
               glm::vec2 direction,
               float speed) :
    instances(instances),
    active_instance(0),
    position(position),
    direction(direction),
    speed(speed),
    passive_instance(0),
    max_passive_time(0.0f)
{
    for (Instance & instance : instances) {
		instance.visible = false;
	}

    Instance & instance = instances[active_instance];
    instance.visible = true;
    update_active_instance();
}

Instance & Player::get_instance() const {
    return instances[active_instance];
}

int Player::get_passive_instance(int time) {
    if (time > max_passive_time && passive_instance != 0) {
        passive_instance = 0;
    }
    return passive_instance;
}

glm::vec2 Player::get_position() const {
    return position;
}

glm::vec2 Player::get_direction() const {
    return direction;
}

float Player::get_speed() const {
    return speed;
}

void Player::set_passive_instance(int index, int time) {
    passive_instance = index;
    max_passive_time = instances[index].get().get_frame_count() + time;
}

void Player::activate_instance(std::size_t index) {
    if (active_instance != index) {

        // Disable old instance
        Instance & old_instance = instances[active_instance];
        old_instance.visible = false;

        // Enable new instance
        active_instance = index;
        Instance & new_instance = instances[index];
        new_instance.visible = true;

        update_active_instance();
    }
}

void Player::update_active_instance() {
    Instance & instance = instances[active_instance];
    instance.position = glm::vec3(position.x, 0.1f, position.y);
    instance.rotation = glm::vec2(0.0f, glm::atan(direction.x, direction.y));
}

void Player::move(glm::vec2 delta) {
	position += delta;
	update_active_instance();
}

void Player::turn(glm::vec2 new_direction) {
	direction = glm::normalize(new_direction);
    update_active_instance();
}
