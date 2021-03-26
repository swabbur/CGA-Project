#pragma once

#include "Model.hpp"
#include "../util/Cache.hpp"
#include <optional>

#include <glm/glm.hpp>

class Instance {

    bool moved;

    std::vector<std::reference_wrapper<Model>> models;

    Instance(std::vector<std::reference_wrapper<Model>> models, Instance &parent);

    explicit Instance(std::vector<std::reference_wrapper<Model>> models);

public:

    static Instance create(Cache<std::string, Model> & cache,
                           std::string const & path,
                           int start,
                           int end,
                           std::string const & suffix);

    static Instance create_animated(std::vector<std::reference_wrapper<Model>> models);

    static Instance create_static(Model &model, Instance &parent);

    static Instance create_static(Model &model);

    std::optional<std::reference_wrapper<Instance>> parent;

	glm::vec3 position;
	glm::vec2 rotation;
	bool visible;
	bool xrayable;

	Instance(Instance const &) = delete;

	Instance(Instance && instance) noexcept;

	~Instance();

	[[nodiscard]] glm::mat4 get_transformation() const;

    [[nodiscard]] int get_frame_count() const;

	Model & get_model(int index);

};