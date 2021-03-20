#pragma once

#include <graphics/Shape.hpp>

class Collision {
public:

	static float swept_AABB(Shape const& o1, Shape const& o2, glm::vec2 position1, glm::vec2 position2, glm::vec2 delta_position, glm::vec2& collision_direction, glm::vec2& collision_distance);

	static glm::vec2 resolve_collision(Shape const& o1, Shape const& o2, glm::vec2 position1, glm::vec2 position2, glm::vec2 delta_position);
};

