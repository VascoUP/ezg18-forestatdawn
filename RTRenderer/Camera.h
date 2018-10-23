#pragma once

#include <GL\glew.h>

#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\type_ptr.hpp>

#include "Transform.h"
#include "ObjectBehavior.h"
#include "Time.h"
#include "Input.h"
#include "GLWindow.h"

class Camera: public ObjectBehavior
{
private:
	static Camera* camera;
	Camera(Transform* object, GLWindow* window);

	glm::mat4 projectionMatrix;
public:
	static Camera* CreateInstance(Transform* object, GLWindow* window);
	static Camera* GetInstance();

	glm::mat4 CalculateViewMatrix();
	glm::mat4 ProjectionMatrix();

	void SetUp();
	void Update();

	~Camera();
};