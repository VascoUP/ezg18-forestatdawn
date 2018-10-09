#include "SceneObject.h"

int SceneObject::NEXT_ID = 1;

SceneObject::SceneObject(Transform* transform)
{
	this->transform = transform;
}

void SceneObject::SetUp() {
	for (std::vector<Updatable*>::iterator it = updatables.begin(); it != updatables.end(); it++)
	{
		(*it)->SetUp();
	}
}

void SceneObject::Update() {
	// Skip Update if the object is not active
	if (!active)
		return;

	for (std::vector<Updatable*>::iterator it = updatables.begin(); it != updatables.end(); it++)
	{
		(*it)->Update();
	}
}

void SceneObject::SetActive(bool active) {
	this->active = active;
}

void SceneObject::AddUpdatable(Updatable* updatable) {
	updatables.push_back(updatable);
}

void SceneObject::RemoveUpdatable(Updatable* updatable) {
	//updatables.erase(std::remove_if(std::begin(updatables), std::end(updatables), updatable), std::end(updatables));
}

Transform* SceneObject::GetTransform() {
	return transform;
}

SceneObject::~SceneObject()
{
	free(transform);
}
