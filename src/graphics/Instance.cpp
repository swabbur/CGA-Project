#include <graphics/Instance.hpp>
#include <glm/gtx/transform.hpp>
#include <utility>

Instance Instance::create(Cache<std::string, Model> & cache,
                       std::string const & path,
                       int start,
                       int end,
                       std::string const & suffix) {

    std::vector<std::reference_wrapper<Model>> models;
    models.reserve(end - start);
    for (int index = start; index < end; index++) {
        std::string final_path = path + std::to_string(index) + suffix;
        Model & model = cache.get(final_path);
        models.emplace_back(model);
    }
    return Instance(models);
}

Instance Instance::create_animated(std::vector<std::reference_wrapper<Model>> models) {
    return Instance(std::move(models));
}

Instance Instance::create_static(Model &model, Instance &parent) {
    std::vector<std::reference_wrapper<Model>> models;
    models.emplace_back(model);
    return Instance(models, parent);
}

Instance Instance::create_static(Model &model) {
    std::vector<std::reference_wrapper<Model>> models;
    models.emplace_back(model);
    return Instance(models);
}

Instance::Instance(std::vector<std::reference_wrapper<Model>> models, Instance & parent) :
    models(std::move(models)), position(0.0f), rotation(0.0f), visible(true), xrayable(false), parent(parent), moved(false) {}

Instance::Instance(std::vector<std::reference_wrapper<Model>> models) :
    models(std::move(models)), position(0.0f), rotation(0.0f), visible(true), xrayable(false), parent(std::nullopt), moved(false) {}

Instance::Instance(Instance &&instance) noexcept :
    models(std::move(instance.models)), position(instance.position), rotation(instance.rotation), visible(instance.visible), xrayable(instance.xrayable), parent(instance.parent), moved(false) {
    instance.moved = true;
}

Instance::~Instance() = default;

glm::mat4 Instance::get_transformation() const {
	glm::mat4 model_matrix(1.0f);
	model_matrix = glm::translate(model_matrix, position);
	model_matrix = glm::rotate(model_matrix, rotation.x, glm::vec3(1.0f, 0.0f, 0.0f));
	model_matrix = glm::rotate(model_matrix, rotation.y, glm::vec3(0.0f, 1.0f, 0.0f));

	if (parent.has_value()) {
	    if (parent.value().get().moved) {
	        throw std::runtime_error("Accessing moved instance");
	    }
	    return parent.value().get().get_transformation() * model_matrix;
	}

	return model_matrix;
}

Model& Instance::get_model(int index) {
    if (models.size() > 1) {
        return models[index % models.size()];
    }
	return models[0];
}
