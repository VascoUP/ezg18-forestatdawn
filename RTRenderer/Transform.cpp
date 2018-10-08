#include "Transform.h"

Transform::Transform()
{
	parent = NULL;
}

Transform::Transform(Transform* parent) {
	this->parent = parent;
}

void Transform::Translate(glm::vec3 translate) {
	model = glm::translate(model, glm::vec3(-1.0f, 0.0f, 2.5f));
}

void Transform::TranslateLocal(glm::vec3 translate) {
	glm::mat4 tmp;
	tmp = glm::translate(tmp, glm::vec3(-1.0f, 0.0f, 2.5f));
	model = glm::matrixCompMult(tmp, model);
}

void Transform::Scale(glm::vec3 scale) {
	model = glm::scale(model, glm::vec3(-1.0f, 0.0f, 2.5f));
}

void Transform::Rotate(float angle, glm::vec3 axis) {
	model = glm::rotate(model, angle, axis);
}

void Transform::RotateAround(glm::vec3 point, glm::vec3 angle, glm::vec3 axis) {

}

glm::mat4 Transform::ToLocalCoordinates(glm::vec3 coordinates) {
	return glm::inverse(model);
}

glm::mat4 Transform::ToWorldCoordinates(glm::vec3 coordinates) {
	return model;
}

Transform::~Transform()
{
}
