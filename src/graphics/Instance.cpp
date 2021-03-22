#include <graphics/Instance.hpp>
#include <glm/gtx/transform.hpp>

Instance::Instance(std::string const path, int start, int end, std::string suffix, Cache<std::string, Model>& model_cache) : position(0.0f), rotation(0.0f), animated(true) {
	for (int i = start; i < end; i++) {
		std::string final_path = path + std::to_string(i) + suffix;
		models.push_back(std::move(model_cache.get(final_path)));
	}
}

Instance::Instance(std::string path, Cache<std::string, Model> & model_cache) : position(0.0f), rotation(0.0f), animated(false) {
	models.push_back(std::move(model_cache.get(path)));
}

Instance::~Instance() = default;

glm::mat4 Instance::get_transformation() const {
	glm::mat4 model_matrix(1.0f);
	model_matrix = glm::translate(model_matrix, position);
	model_matrix = glm::rotate(model_matrix, rotation.x, glm::vec3(1.0f, 0.0f, 0.0f));
	model_matrix = glm::rotate(model_matrix, rotation.y, glm::vec3(0.0f, 1.0f, 0.0f));
	return model_matrix;
}

Model& Instance::get_model(int index) {
	if (animated) {
		return models[index % models.size()];
	}
	else {
		return models[0];
	}
}
