#include <physics/AABB.hpp>

AABB::AABB(glm::vec2 min, glm::vec2 max) : minima(min), maxima(max) {}

AABB::AABB(AABB const& aabb) : minima(aabb.minima), maxima(aabb.maxima) {}

glm::vec2 AABB::get_minima() const {
	return minima;
}

glm::vec2 AABB::get_maxima() const {
	return maxima;
}
