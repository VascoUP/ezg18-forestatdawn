#include "RotationObjects.h"


RotatingObject::RotatingObject(Transform* container, float velX, float velY, float velZ)
	: AObjectBehavior(container)
{
	rotationVelocityX = velX;
	rotationVelocityY = velY;
	rotationVelocityZ = velZ;
}

void RotatingObject::SetUp() {}

void RotatingObject::Update() {
	transform->Rotate(rotationVelocityX * (GLfloat)Time::GetDeltaTime(), rotationVelocityY * (GLfloat)Time::GetDeltaTime(), rotationVelocityZ * (GLfloat)Time::GetDeltaTime());
}

RotatingObject::~RotatingObject()
{
}
