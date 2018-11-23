#include "AObjectBehavior.h"

AObjectBehavior::AObjectBehavior(Transform *container)
{
	this->transform = container;
}

Transform * AObjectBehavior::GetTransform()
{
	return transform;
}

AObjectBehavior::~AObjectBehavior()
{
}
