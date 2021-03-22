#pragma once

#include "Model.hpp"
#include "../util/Cache.hpp"

#include <glm/glm.hpp>

class Instance {

	std::vector<Model> models;

public:

	glm::vec3 position;
	glm::vec2 rotation;
	bool animated;

	explicit Instance(std::string const path, int start, int end, std::string suffix, Cache<std::string, Model>& model_cache);

	explicit Instance(std::string path, Cache<std::string, Model>& model_cache);

	Instance(Instance &&) = default;

	~Instance();

	[[nodiscard]] glm::mat4 get_transformation() const;

	Model & get_model(int index);

};