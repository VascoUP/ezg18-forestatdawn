#pragma once

#include <stdio.h>
#include <vector> 
#include <algorithm>

#include "Transform.h"
#include "Updatable.h"

class SceneObject: public Updatable
{
private:
	static int NEXT_ID;
	const int OBJECT_ID = NEXT_ID++;

	bool active = true;
	Transform* transform;
	std::vector<Updatable*> updatables;

public:
	SceneObject(Transform* transform);

	void SetUp();
	void Update();

	void SetActive(bool active);

	Transform* GetTransform();

	void AddUpdatable(Updatable* updatable);
	void RemoveUpdatable(Updatable* updatable);

	~SceneObject();
};

