#pragma once

#include <glm/glm.hpp>
#include "Model.hpp"

class Entity{

public:

	Model const & scene;
	glm::vec3 position = glm::vec3(0.0f);
	glm::vec3 rotation = glm::vec3(0.0f);

	Entity(Model const & scene);

	~Entity();

	glm::mat4 const get_model_matrix();

};