#pragma once

#include <glm/glm.hpp>

class BoundingBox {

    glm::vec2 minima;
    glm::vec2 maxima;
    float height;

public:

    BoundingBox(glm::vec2 min, glm::vec2 max, float height);

    [[nodiscard]] glm::vec2 get_minima() const;

    [[nodiscard]] glm::vec2 get_maxima() const;

    [[nodiscard]] float get_height() const;

    [[nodiscard]] BoundingBox flip() const;

};
