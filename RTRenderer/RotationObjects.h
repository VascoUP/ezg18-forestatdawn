#pragma once

#include "Transform.h"
#include "ObjectBehavior.h"
#include "Time.h"


class RotatingObject : public ObjectBehavior
{
private:
	float rotationVelocityX;
	float rotationVelocityY;
	float rotationVelocityZ;
public:
	RotatingObject(Transform* container, float velX, float velY, float velZ);

	void Update();

	~RotatingObject();
};

