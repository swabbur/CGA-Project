#include <physics/BoundingBox.hpp>

BoundingBox::BoundingBox(glm::vec2 min, glm::vec2 max, float height) : minima(min), maxima(max), height(height) {}

glm::vec2 BoundingBox::get_minima() const {
	return minima;
}

glm::vec2 BoundingBox::get_maxima() const {
	return maxima;
}

float BoundingBox::get_height() const {
    return height;
}

BoundingBox BoundingBox::flip() {
    return BoundingBox(glm::vec2(minima[1], minima[0]), glm::vec2(maxima[1], maxima[0]), height);
}
