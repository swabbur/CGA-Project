#pragma once

#include <glm/glm.hpp>

class AABB {

    glm::vec2 minima;
    glm::vec2 maxima;

public:

    AABB(glm::vec2 min, glm::vec2 max);

    [[nodiscard]] glm::vec2 get_minima() const;

    [[nodiscard]] glm::vec2 get_maxima() const;

};
