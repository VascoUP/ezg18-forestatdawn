#pragma once

#include <GL\glew.h>

#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>

#include "Updatable.h"
#include "Time.h"
#include "Input.h"

class Camera: public Updatable
{
private:
	static Camera* camera;
	Camera(glm::vec3 position, glm::vec3 up, GLfloat yaw, GLfloat pitch, GLfloat moveSpeed, GLfloat turnSpeed);

	glm::vec3 position;
	glm::vec3 front;
	glm::vec3 right;
	glm::vec3 up;
	glm::vec3 worldUp;

	GLfloat yaw;
	GLfloat pitch;

	GLfloat moveSpeed;
	GLfloat turnSpeed;
public:
	static Camera* CreateInstance(glm::vec3 position, glm::vec3 up, GLfloat yaw, GLfloat pitch, GLfloat moveSpeed, GLfloat turnSpeed);
	static Camera* GetInstance();

	glm::mat4 CalculateViewMatrix();
	void SetUp();
	void KeyControl();
	void MouseControl();
	void Update();

	~Camera();
};