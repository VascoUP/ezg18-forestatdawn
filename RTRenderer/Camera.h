#pragma once

#include <GL\glew.h>

#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>

#include "SceneObject.h"
#include "ObjectBehavior.h"
#include "Time.h"
#include "Input.h"

class Camera: public ObjectBehavior
{
private:
	static Camera* camera;
	Camera(const SceneObject* object, glm::vec3 position, glm::vec3 up, GLfloat yaw, GLfloat pitch);

	glm::vec3 _worldUp;
public:
	glm::vec3 position;
	glm::vec3 front;
	glm::vec3 right;
	glm::vec3 up;

	GLfloat yaw;
	GLfloat pitch;

	static Camera* CreateInstance(const SceneObject* object, glm::vec3 position, glm::vec3 up, GLfloat yaw, GLfloat pitch);
	static Camera* GetInstance();

	glm::mat4 CalculateViewMatrix();

	void SetUp();
	void Update();

	~Camera();
};