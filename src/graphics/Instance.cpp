#include <graphics/Instance.hpp>
#include <glm/gtx/transform.hpp>

Instance::Instance(Model const & scene) : model(scene), position(0.0f), rotation(0.0f) {}

Instance::~Instance() = default;

glm::mat4 Instance::get_transformation() const {
	glm::mat4 model_matrix(1.0f);
	model_matrix = glm::translate(model_matrix, position);
	model_matrix = glm::rotate(model_matrix, rotation.x, glm::vec3(1.0f, 0.0f, 0.0f));
	model_matrix = glm::rotate(model_matrix, rotation.y, glm::vec3(0.0f, 1.0f, 0.0f));
	return model_matrix;
}


