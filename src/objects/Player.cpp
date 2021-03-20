#include <objects/Player.hpp>

Player::Player(Instance& instance, glm::vec2 position) : movable(Movable(instance, position)) {}