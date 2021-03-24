#pragma once

#include "Model.hpp"
#include "../util/Cache.hpp"

#include <glm/glm.hpp>

class Instance {

	std::vector<std::reference_wrapper<Model>> models;

public:

    static Instance create(Cache<std::string, Model> & cache,
                           std::string const & path,
                           int start,
                           int end,
                           std::string const & suffix);

	glm::vec3 position;
	glm::vec2 rotation;
	bool visible;
	bool xrayable;

	explicit Instance(std::vector<std::reference_wrapper<Model>> models);

	explicit Instance(Model & model);

	Instance(Instance &&) = default;

	~Instance();

	[[nodiscard]] glm::mat4 get_transformation() const;

	Model & get_model(int index);

};