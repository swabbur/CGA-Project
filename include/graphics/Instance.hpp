#pragma once

#include "Model.hpp"

#include <glm/glm.hpp>

class Instance {

public:

	Model const & model;
	glm::vec3 position;
	glm::vec2 rotation;

	explicit Instance(Model const & scene);

	~Instance();

	[[nodiscard]] glm::mat4 get_transformation() const;

};