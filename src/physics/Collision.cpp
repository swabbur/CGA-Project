#include <physics/Collision.hpp>
#include <iostream>

float Collision::swept_AABB(Shape const & o1, Shape const & o2, glm::vec2 position1, glm::vec2 position2, glm::vec2 d1, glm::vec2 d2, glm::vec2 delta_position, glm::vec2 & collision_direction, glm::vec2& collision_distance, bool rotation_invariant1) {

    // Set up preliminary values
	BoundingBox bounding_box1(o1.get_bounding_box());
	BoundingBox bounding_box2(o2.get_bounding_box());

	if (rotation_invariant1) {
		float minima = std::min(bounding_box1.get_minima().x, bounding_box1.get_minima().y);
		float maxima = std::max(bounding_box1.get_maxima().x, bounding_box1.get_maxima().y);
		bounding_box1 = BoundingBox(glm::vec2(minima), glm::vec2(maxima), bounding_box1.get_height());
	}
	else if (std::abs(d1[0]) < std::abs(d1[1])) {
	    bounding_box1 = bounding_box1.flip();
	}
    if (std::abs(d2[0]) < std::abs(d2[1])) {
        bounding_box2 = bounding_box2.flip();
    }

	glm::vec2 pos1 = bounding_box1.get_minima() + position1;
	glm::vec2 pos2 = bounding_box2.get_minima() + position2;
	glm::vec2 size1 = bounding_box1.get_maxima() - bounding_box1.get_minima();
	glm::vec2 size2 = bounding_box2.get_maxima() - bounding_box2.get_minima();

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
		bool betweenx = pos1.x < pos2.x+size2.x && pos1.x + size1.x > pos2.x;
		entry_time = glm::vec2(betweenx ? -std::numeric_limits<float>::infinity() : std::numeric_limits<float>::infinity(), collision_distance.y / delta_position.y);
		exit_time = glm::vec2(betweenx ? std::numeric_limits<float>::infinity() : -std::numeric_limits<float>::infinity(), exit_distance.y / delta_position.y);
	}
	else if (delta_position.y == 0.0f) {
		bool betweeny = pos1.y < pos2.y + size2.y && pos1.y + size1.y > pos2.y;
		entry_time = glm::vec2(collision_distance.x / delta_position.x, betweeny ? -std::numeric_limits<float>::infinity() : std::numeric_limits<float>::infinity());
		exit_time = glm::vec2(exit_distance.x / delta_position.x, betweeny ? std::numeric_limits<float>::infinity() : -std::numeric_limits<float>::infinity());
	}
	else {
		entry_time = collision_distance / delta_position;
		exit_time = exit_distance / delta_position;
	}

	// Check for collision
	float max_entry_time = std::max(entry_time.x, entry_time.y);
	float min_exit_time = std::min(exit_time.x, exit_time.y);

	if (max_entry_time > min_exit_time || min_exit_time <= 0.0f || max_entry_time >= 1.0f) {
		return 1.0f;
	}

	// Calculate the collision normals
	if (std::abs(collision_distance.x) > std::abs(exit_distance.x)) {
		collision_distance.x = exit_distance.x;
	}

	if (std::abs(collision_distance.y) > std::abs(exit_distance.y)) {
		collision_distance.y = exit_distance.y;
	}

	collision_direction = (std::abs(collision_distance.x) < std::abs(collision_distance.y) ? glm::vec2(1.0f, 0.0f) : glm::vec2(0.0f, 1.0f));

	return max_entry_time;
}

glm::vec2 Collision::resolve_collision(Shape const& o1, Shape const& o2, glm::vec2 position1, glm::vec2 position2, glm::vec2 d1, glm::vec2 d2, glm::vec2 delta_position) {

	glm::vec2 collision_direction(0.0f);
	glm::vec2 collision_distance(0.0f);
	float collision_time = swept_AABB(o1, o2, position1, position2, d1, d2, delta_position, collision_direction, collision_distance, true);

	if (collision_time >= 1.0f) {
		return delta_position;
	}

	glm::vec2 other_direction = glm::vec2(1.0f, 1.0f) - collision_direction;
	glm::vec2 final_delta_position = (collision_direction * (collision_distance + (glm::normalize(collision_distance * collision_direction) * glm::vec2(1e-4)))) + (other_direction * delta_position);

	return final_delta_position;
}
