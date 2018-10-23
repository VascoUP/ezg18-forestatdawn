#include "Transform.h"

int Transform::NEXT_ID = 1;

Transform::Transform() {
	parent = NULL;
	InitializeVariables();
}

Transform::Transform(Transform* parent) {
	this->parent = parent;
	this->parent->AddChild(this);
	InitializeVariables();
}

void Transform::InitializeVariables() {
	children = std::vector<Transform*>();

	position = glm::vec3(0, 0, 0);
	rotation = glm::vec3(0, 0, 0);
	scale = glm::vec3(1, 1, 1);

	if (this->parent != NULL) {
		worldTransformation = parent->GetWorldTransform();
	}

	UpdateRotationAxis();
	TransformMatrix(false);
}


glm::vec3 Transform::GetPosition() const { return position; }

glm::vec3 Transform::GetFront() const { return front; }

glm::vec3 Transform::GetUp() const { return up; }

std::vector<Transform*> Transform::GetChildren() { return children; }

void Transform::AddChild(Transform* child) { children.push_back(child); }


void Transform::SetUp() {
	for (std::vector<Updatable*>::iterator it = updatables.begin(); it != updatables.end(); it++)
	{
		(*it)->SetUp();
	}

	// Iterate over children
	for (std::vector<Transform*>::iterator it = children.begin(); it != children.end(); it++)
	{
		(*it)->SetUp();
	}
}

void Transform::Update() {
	// Skip Update if the object is not active
	if (!active)
		return;

	for (std::vector<Updatable*>::iterator it = updatables.begin(); it != updatables.end(); it++)
	{
		(*it)->Update();
	}

	// Iterate over children
	for (std::vector<Transform*>::iterator it = children.begin(); it != children.end(); it++)
	{
		(*it)->Update();
	}
}

void Transform::Render() {
	if (!active)
		return;

	for (std::vector<Renderable*>::iterator it = renderables.begin(); it != renderables.end(); it++)
	{
		(*it)->Render();
	}

	// Iterate over children
	for (std::vector<Transform*>::iterator it = children.begin(); it != children.end(); it++)
	{
		(*it)->Render();
	}
}

void Transform::SetActive(bool active) {
	this->active = active;
}

void Transform::AddUpdatable(Updatable* updatable) {
	updatables.push_back(updatable);
}

void Transform::AddRenderable(Renderable* renderable) {
	renderables.push_back(renderable);
}


void Transform::Translate(glm::vec3 translate) {
	this->position += right * translate.x + up * translate.y + front * translate.z;
}

void Transform::Scale(glm::vec3 scale) {
	this->scale *= scale;
}

void Transform::Rotate(glm::vec3 axis) {
	this->rotation += axis;
	UpdateRotationAxis();
}

glm::mat3 Transform::CalculateRotationMatrix() {
	float arrMatRoll[9] = {
		cos(rotation.z),	-sin(rotation.z),	0,
		sin(rotation.z),	cos(rotation.z),	0,
		0,					0,					1
	};
	glm::mat3 matRoll = glm::make_mat3(arrMatRoll);

	float arrMatYaw[9] = {
		cos(-rotation.y),	0,	sin(-rotation.y),
		0,					1,	0,
		-sin(-rotation.y),	0,	cos(-rotation.y)
	};
	glm::mat3 matYaw = glm::make_mat3(arrMatYaw);

	float arrMatPitch[9] = {
		1, 0,				0,
		0, cos(rotation.x),	-sin(rotation.x),
		0, sin(rotation.x),	cos(rotation.x)
	};
	glm::mat3 matPitch = glm::make_mat3(arrMatPitch);

	return matRoll * matYaw * matPitch;
}

void Transform::UpdateRotationAxis() {	
	float arrAxis[9] = {
		1,0,0,
		0,1,0,
		0,0,1
	};
	glm::mat3 axis = glm::make_mat3(arrAxis);

	axis = axis * CalculateRotationMatrix();

	front = axis[2];
	right = axis[0];
	up = axis[1];
}

glm::mat4 Transform::GetWorldTransform() {
	return worldTransformation;
}

glm::mat4 Transform::TransformMatrix(bool doScale) {
	/*
	float cosYaw = cos(rotation.z);
	float sinYaw = sin(rotation.z);
	float cosPitch = cos(rotation.y);
	float sinPitch = sin(rotation.y);
	float cosRoll = cos(rotation.x);
	float sinRoll = sin(rotation.x);

	float arrTransform[16] = {
		cosYaw * cosPitch,	cosYaw * sinPitch * sinRoll - sinYaw * cosRoll,		cosYaw * sinPitch * cosRoll + sinYaw * sinRoll,		0,
		sinYaw * cosPitch,	sinYaw * sinPitch * sinRoll + cosYaw * cosRoll,		sinYaw * sinPitch * cosRoll - cosYaw * sinRoll,		0,
		-sinPitch,			cosPitch * sinRoll,									cosPitch * cosRoll,									0,
		0,					0,													0,													1
	};
	localTransformation = glm::make_mat4(arrTransform);
	glm::translate(localTransformation, position);
	*/
	//localTransformation = glm::lookAt(position, position + front, up);

	localTransformation = glm::mat4();
	localTransformation = glm::translate(localTransformation, position);
	localTransformation = glm::rotate(localTransformation, rotation.x, glm::vec3(1, 0, 0));
	localTransformation = glm::rotate(localTransformation, rotation.y, glm::vec3(0, 1, 0));
	localTransformation = glm::rotate(localTransformation, rotation.z, glm::vec3(0, 0, 1));
		
	if (parent == NULL) {
		worldTransformation = localTransformation;
	}
	else {
		worldTransformation = parent->GetWorldTransform() * localTransformation;
	}

	if (!doScale)
		return worldTransformation;

	glm::mat4 scaleMat = glm::scale(glm::mat4(), this->scale);
	return worldTransformation * scaleMat;
}

Transform::~Transform()
{
}
