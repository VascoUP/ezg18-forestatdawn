#pragma once

#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>

class Transform
{
	Transform* parent;
	glm::mat4 model;

public:
	Transform();
	Transform(Transform* parent);

	void Translate(glm::vec3 translate);
	void TranslateLocal(glm::vec3 translate);
	void Scale(glm::vec3 scale);
	void Rotate(float angle, glm::vec3 axis);
	void RotateAround(glm::vec3 point, glm::vec3 angle, glm::vec3 axis);

	glm::mat4 ToLocalCoordinates(glm::vec3 coordinates);
	glm::mat4 ToWorldCoordinates(glm::vec3 coordinates);

	~Transform();
};

