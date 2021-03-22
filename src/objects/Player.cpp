#include <objects/Player.hpp>

Player::Player(Instance& instance, glm::vec2 position) : movable(Movable(instance, position)) {}

Player::Player(std::vector<Instance*> instances, glm::vec2 position) : movable(Movable(instances, position)) {}