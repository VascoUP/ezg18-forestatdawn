#pragma once

#include "AObjectBehavior.h"
#include "Transform.h"
#include "Time.h"
#include "Input.h"

class ObjectController : public AObjectBehavior
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

