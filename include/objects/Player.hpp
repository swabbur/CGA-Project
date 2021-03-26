#pragma once

#include "../graphics/Instance.hpp"

#include <glm/glm.hpp>

class Player {

    std::vector<std::reference_wrapper<Instance>> instances;
    std::size_t active_instance;

	glm::vec2 position;
	glm::vec2 direction;
    float speed;
    float run_speed;
    float max_passive_time;
    int passive_instance;

	void update_active_instance();

public:

    Player(std::vector<std::reference_wrapper<Instance>> const & instances,
           glm::vec2 position,
           glm::vec2 direction,
           float speed,
           float run_speed);

    [[nodiscard]] Instance & get_instance() const;

    [[nodiscard]] int get_passive_instance(int time);

    [[nodiscard]] glm::vec2 get_position() const;

    [[nodiscard]] glm::vec2 get_direction() const;

	[[nodiscard]] float get_speed() const;

    [[nodiscard]] float get_run_speed() const;

    void set_passive_instance(int index, int time);

    void activate_instance(std::size_t index);

    void move(glm::vec2 delta);

    void turn(glm::vec2 new_direction);
};