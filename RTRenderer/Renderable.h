#pragma once

#include <stdio.h>

#include "Transform.h"

class Renderable
{
private:
	Transform* transform;
public:
	Renderable(Transform* container);

	Transform* GetTransform();

	virtual void Load();
	virtual void Render();
	virtual void Clear();

	~Renderable();
};