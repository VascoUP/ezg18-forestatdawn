#pragma once

#include "AObjectBehavior.h"
#include "Transform.h"
#include "Time.h"


class RotatingObject : public AObjectBehavior
{
private:
	float rotationVelocityX;
	float rotationVelocityY;
	float rotationVelocityZ;
public:
	RotatingObject(Transform* container, float velX, float velY, float velZ);

	void SetUp();
	void Update();

	~RotatingObject();
};

