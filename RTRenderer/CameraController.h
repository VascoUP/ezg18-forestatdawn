#pragma once

#include "AObjectBehavior.h"
#include "Transform.h"
#include "Camera.h"

class CameraController : public AObjectBehavior
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

