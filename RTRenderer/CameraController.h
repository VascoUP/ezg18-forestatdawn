#pragma once

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
	CameraController(const SceneObject* container, Camera* camera, GLfloat moveSpeed, GLfloat turnSpeed);

	void SetUp();
	void Update();

	~CameraController();
};

