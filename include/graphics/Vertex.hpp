#pragma once

#include <glm/glm.hpp>

template<std::size_t N>
class Vertex;

template<>
class Vertex<2> {

public:

    glm::vec2 position;
    glm::vec2 texture_coordinates;

    Vertex(glm::vec2 position, glm::vec2 texture_coordinates) :
        position(position), texture_coordinates(texture_coordinates) {}
};

template<>
class Vertex<3> {

public:

    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 texture_coordinates;

    Vertex() : position(), normal(), texture_coordinates() {}
};