#pragma once

#include <graphics/Shape.hpp>

namespace Collision {

    float swept_AABB(Shape const& o1, Shape const& o2, glm::vec2 position1, glm::vec2 position2, glm::vec2 d1, glm::vec2 d2, glm::vec2 delta_position, glm::vec2& collision_direction, glm::vec2& collision_distance, bool rotation_invariant1);

    glm::vec2 resolve_collision(Shape const& o1, Shape const& o2, glm::vec2 position1, glm::vec2 position2, glm::vec2 d1, glm::vec2 d2, glm::vec2 delta_position);
}
