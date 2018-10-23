#pragma once

#include "Transform.h"
#include "ObjectBehavior.h"
#include "Camera.h"

class CameraController : public ObjectBehavior
{
private:
	Camera * camera;

	GLfloat moveSpeed;
	GLfloat turnSpeed;

	void KeyControl();
	void MouseControl();

public:
	CameraController(Transform* container, Camera* camera, GLfloat moveSpeed, GLfloat turnSpeed);

	void SetUp();
	void Update();

	~CameraController();
};

