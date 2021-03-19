#include <graphics/Entity.hpp>
#include <glm/gtx/transform.hpp>

Entity::Entity(Model const & scene) : scene(scene) {}

Entity::~Entity() = default;

glm::mat4 const Entity::get_model_matrix() {
	glm::mat4 model_matrix(1.0f);
	model_matrix = glm::translate(model_matrix, position);
	model_matrix = glm::rotate(model_matrix, rotation.x, glm::vec3(1.0f, 0.0f, 0.0f));
	model_matrix = glm::rotate(model_matrix, rotation.y, glm::vec3(0.0f, 1.0f, 0.0f));
	model_matrix = glm::rotate(model_matrix, rotation.z, glm::vec3(0.0f, 0.0f, 1.0f));
	return model_matrix;
}