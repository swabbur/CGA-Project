#include <physics/AABB.hpp>

AABB::AABB(glm::vec2 min, glm::vec2 max, float height) : minima(min), maxima(max), height(height) {}

glm::vec2 AABB::get_minima() const {
	return minima;
}

glm::vec2 AABB::get_maxima() const {
	return maxima;
}

float AABB::get_height() const {
    return height;
}

AABB AABB::flip() {
    return AABB(glm::vec2(minima[1], minima[0]), glm::vec2(maxima[1], maxima[0]), height);
}
