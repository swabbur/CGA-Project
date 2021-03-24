#pragma once

#include <glm/glm.hpp>

class AABB {

    glm::vec2 minima;
    glm::vec2 maxima;
    float height;

public:

    AABB(glm::vec2 min, glm::vec2 max, float height);
    AABB(AABB const & aabb);

    glm::vec2 get_minima() const;

    glm::vec2 get_maxima() const;

    float get_height() const;

    AABB flip();

};
