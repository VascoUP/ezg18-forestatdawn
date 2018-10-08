#include "SceneObject.h"

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
	for (std::vector<Updatable*>::iterator it = updatables.begin(); it != updatables.end(); it++)
	{
		(*it)->Update();
	}
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
