#include <physics/AABB.hpp>

AABB::AABB(glm::vec2 min, glm::vec2 max, float height) : minima(min), maxima(max), height(height) {}

AABB::AABB(AABB const& aabb) : minima(aabb.minima), maxima(aabb.maxima), height(aabb.height) {}

glm::vec2 AABB::get_minima() const {
	return minima;
}

glm::vec2 AABB::get_maxima() const {
	return maxima;
}

float AABB::get_height() const {
    return height;
}
