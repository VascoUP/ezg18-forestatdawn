#pragma once

#include "IUpdatable.h"
#include "Transform.h"

class AObjectBehavior : public IUpdatable
{
protected:
	Transform* transform;
public:
	AObjectBehavior(Transform* container);

	Transform* GetTransform();

	~AObjectBehavior();
};

