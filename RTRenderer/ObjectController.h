#pragma once

#include "Transform.h"
#include "ObjectBehavior.h"
#include "Time.h"
#include "Input.h"

class ObjectController : public ObjectBehavior
{
private:
	float moveSpeed;
	float rotSpeed;
public:
	ObjectController(Transform* container, float vel, float rotVel);

	void SetUp();
	void Update();

	~ObjectController();
};

