#include <objects/Player.hpp>

Player::Player(Entity & entity, glm::vec2 position) : movable(Movable(entity, position)) {}