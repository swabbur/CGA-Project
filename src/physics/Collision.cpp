#include <physics/Collision.hpp>

float Collision::swept_AABB(Shape const & o1, Shape const & o2, glm::vec2 position1, glm::vec2 position2, glm::vec2 d1, glm::vec2 d2, glm::vec2 delta_position, glm::vec2 & collision_direction, glm::vec2& collision_distance) {
	// Set up preliminary values
	AABB aabb1 = AABB(o1.get_mesh().get_AABB());
	AABB aabb2 = AABB(o2.get_mesh().get_AABB());

	if (std::abs(d1[0]) < std::abs(d1[1])) {
	    aabb1 = aabb1.flip();
	}
    if (std::abs(d2[0]) < std::abs(d2[1])) {
        aabb2 = aabb2.flip();
    }

	glm::vec2 pos1 = aabb1.get_minima() + position1;
	glm::vec2 pos2 = aabb2.get_minima() + position2;
	glm::vec2 size1 = aabb1.get_maxima() - aabb1.get_minima();
	glm::vec2 size2 = aabb2.get_maxima() - aabb2.get_minima();

	// Don't collide with floors
	if (aabb1.get_height() < .001 || aabb2.get_height() < .001) {
	    return 1.0f;
	}

	// Calculate Shape 1's entry distance
	collision_distance = pos2 - pos1;
	collision_distance.x += (delta_position.x > 0.0f ? -size1.x : size2.x);
	collision_distance.y += (delta_position.y > 0.0f ? -size1.y : size2.y);

	// Calculate Shape 1's exit distance
	glm::vec2 exit_distance = pos2 - pos1;
	exit_distance.x += (delta_position.x > 0.0f ? size2.x : -size1.x);
	exit_distance.y += (delta_position.y > 0.0f ? size2.y : -size1.y);

	// Calculate the entry and exit times
	glm::vec2 entry_time;
	glm::vec2 exit_time;
	if (delta_position.x == 0.0f && delta_position.y == 0.0f) {
		return 1.0f;
	}
	else if (delta_position.x == 0.0f) {
		float betweenx = (pos1.x < pos2.x+size2.x && pos1.x + size1.x > pos2.x ? 0.0f : std::numeric_limits<float>::infinity());
		entry_time = glm::vec2(betweenx, collision_distance.y / delta_position.y);
		exit_time = glm::vec2(std::numeric_limits<float>::infinity(), exit_distance.y / delta_position.y);
	}
	else if (delta_position.y == 0.0f) {
		float betweeny = (pos1.y < pos2.y + size2.y && pos1.y + size1.y > pos2.y ? 0.0f : std::numeric_limits<float>::infinity());
		entry_time = glm::vec2(collision_distance.x / delta_position.x, betweeny);
		exit_time = glm::vec2(exit_distance.x / delta_position.x, std::numeric_limits<float>::infinity());
	}
	else {
		entry_time = collision_distance / delta_position;
		exit_time = exit_distance / delta_position;
	}

	// Check for collision
	float max_entry_time = std::max(entry_time.x, entry_time.y);
	float min_exit_time = std::min(exit_time.x, exit_time.y);

	if (max_entry_time > min_exit_time || max_entry_time < 0.0f || max_entry_time > 1.0f) {
		return 1.0f;
	}

	// Calculate the collision normals
	if (entry_time.x > entry_time.y) {
		collision_direction = glm::vec2(1.0f, 0.0f);
	}
	else {
		collision_direction = glm::vec2(0.0f, 1.0f);
	}

	return max_entry_time;
}

glm::vec2 Collision::resolve_collision(Shape const& o1, Shape const& o2, glm::vec2 position1, glm::vec2 position2, glm::vec2 d1, glm::vec2 d2, glm::vec2 delta_position) {

	glm::vec2 collision_direction(0.0f);
	glm::vec2 collision_distance(0.0f);
	float collision_time = swept_AABB(o1, o2, position1, position2, d1, d2, delta_position, collision_direction, collision_distance);

	if (collision_time >= 1.0f) {
		return delta_position;
	}

	glm::vec2 other_direction = glm::vec2(1.0f, 1.0f) - collision_direction;
	glm::vec2 final_delta_position = (collision_direction * (collision_distance + glm::vec2((glm::dot(collision_distance, collision_direction)>0.0f?-1.0f:1.0f)*1e-6))) + (other_direction * delta_position);

	return final_delta_position;
}
