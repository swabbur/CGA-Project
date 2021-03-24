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

Instance::Instance(std::vector<std::reference_wrapper<Model>> models) :
    models(std::move(models)), position(0.0f), rotation(0.0f), visible(true), xrayable(false)  {}

Instance::Instance(Model & model) : Instance(std::vector<std::reference_wrapper<Model>>{model}) {}

Instance::~Instance() = default;

glm::mat4 Instance::get_transformation() const {
	glm::mat4 model_matrix(1.0f);
	model_matrix = glm::translate(model_matrix, position);
	model_matrix = glm::rotate(model_matrix, rotation.x, glm::vec3(1.0f, 0.0f, 0.0f));
	model_matrix = glm::rotate(model_matrix, rotation.y, glm::vec3(0.0f, 1.0f, 0.0f));
	return model_matrix;
}

Model& Instance::get_model(int index) {
    if (models.size() > 1) {
        return models[index % models.size()];
    }
	return models[0];
}
