#include "Transform.h"

int Transform::NEXT_ID = 1;

Transform::Transform() {
	parent = NULL;
	_InitializeVariables();
}

Transform::Transform(Transform* parent) {
	this->parent = parent;
	this->parent->AddChild(this);
	_InitializeVariables();
}

void Transform::_InitializeVariables() {
	m_static = _GetParentsAreStatic(parent);
	
	m_position = glm::vec3(0.0f, 0.0f, 0.0f);
	m_pitch = 0.0f; m_yaw = 0.0f; m_roll = 0.0f;
	m_scale = 1.0f;

	if (this->parent != NULL) {
		m_worldMatrix = parent->GetWorldMatrix();
	}

	_UpdateRotationAxis();
	TransformMatrix(false);
}


void Transform::_SetChildrenDynamic(Transform* transform)
{
	transform->m_static = false;
	for (size_t i = 0; i < transform->m_children.size(); i++) {
		_SetChildrenDynamic(transform->m_children[i]);
	}
}

bool Transform::_GetParentsAreStatic(Transform* transform)
{
	return transform == NULL || (transform->m_static && _GetParentsAreStatic(transform->parent));
}

bool Transform::SetStatic(bool isStatic) {
	// Transform all children transforms if !isStatic
	if (!isStatic) {
		_SetChildrenDynamic(this); 
		return true;
	}
	// This transform can only be static if none of the parent objects are dynamic
	else if (parent != NULL && _GetParentsAreStatic(parent)) {
		m_static = isStatic; 
		return true;
	}
	return false;
}

bool Transform::GetStatic() const
{
	return m_static;
}

glm::vec3 Transform::GetPosition() { return m_position; }

glm::vec3 Transform::GetWorldPosition()
{
	glm::mat4 lMat = glm::mat4();
	lMat = glm::translate(lMat, m_front);
	lMat = m_worldMatrix * lMat;
	return glm::vec3(lMat[3][0], lMat[3][1], lMat[3][2]);
}

glm::vec3 Transform::GetFront() { return m_front; }

glm::vec3 Transform::GetUp() { return m_up; }

glm::vec3 Transform::GetWorldUp()
{
	/*glm::vec4 tmp = glm::vec4(up.x, up.y, up.z, 1.0f);
	tmp = parent != NULL ? parent->GetWorldMatrix() * tmp : tmp;
	glm::vec3 res = glm::vec3(tmp.x, tmp.y, tmp.z);
	return res - GetWorldPosition();*/
	glm::mat4 lMat = glm::mat4();
	lMat = glm::translate(lMat, m_up);
	lMat = m_worldMatrix * lMat;
	glm::vec3 wPoint = glm::vec3(lMat[3][0], lMat[3][1], lMat[3][2]);
	glm::vec3 wTranslation = glm::vec3(m_worldMatrix[3][0], m_worldMatrix[3][1], m_worldMatrix[3][2]);
	return glm::normalize(wPoint - wTranslation);
}

glm::mat4 Transform::GetWorldMatrix() const {
	return m_worldMatrix;
}

glm::mat4 Transform::GetLocalMatrix() const
{
	return m_localMatrix;
}


glm::vec3 Transform::LocalToWorldCoordinates(glm::vec3 point, POINT_TYPE type)
{
	glm::mat4 lMat = glm::mat4();
	lMat = glm::translate(lMat, point);
	lMat = m_worldMatrix * lMat;
	point = glm::vec3(lMat[3][0], lMat[3][1], lMat[3][2]);
	if (type == POINT) return point;
	glm::vec3 wTranslation = glm::vec3(m_worldMatrix[3][0], m_worldMatrix[3][1], m_worldMatrix[3][2]);
	return glm::normalize(point - wTranslation);
}


std::vector<Transform*> Transform::GetChildren() { return m_children; }

void Transform::AddChild(Transform* child) { m_children.push_back(child); }


void Transform::SetUp() {
	for (std::vector<IUpdatable*>::iterator it = m_updatables.begin(); it != m_updatables.end(); it++)
	{
		(*it)->SetUp();
	}

	// Iterate over children
	for (std::vector<Transform*>::iterator it = m_children.begin(); it != m_children.end(); it++)
	{
		(*it)->SetUp();
	}
}

void Transform::Update() {
	for (std::vector<IUpdatable*>::iterator it = m_updatables.begin(); it != m_updatables.end(); it++)
	{
		(*it)->Update();
	}

	// Iterate over children
	for (std::vector<Transform*>::iterator it = m_children.begin(); it != m_children.end(); it++)
	{
		(*it)->Update();
	}
}

void Transform::AddUpdatable(IUpdatable* updatable) {
	m_updatables.push_back(updatable);
}

void Transform::_PropagateWorldMatrix()
{
	for (size_t i = 0; i < m_children.size(); i++) {
		m_children[i]->_PropagateWorldMatrix(m_worldMatrix);
	}
}

void Transform::_PropagateWorldMatrix(glm::mat4 parentWorldMatrix)
{
	m_worldMatrix = parentWorldMatrix * m_localMatrix;
	// Depth first algorithm
	for (size_t i = 0; i < m_children.size(); i++) {
		m_children[i]->_PropagateWorldMatrix(m_worldMatrix);
	}
}

void Transform::_UpdateLocalMatrix()
{
	m_localMatrix = glm::mat4(1.0f);
	m_localMatrix = glm::translate(m_localMatrix, m_position);
	m_localMatrix = m_localMatrix * glm::yawPitchRoll(m_yaw, m_pitch, m_roll);
}

void Transform::_UpdateWolrdMatrix()
{
	m_worldMatrix = parent != NULL ? parent->GetWorldMatrix() * m_localMatrix : m_localMatrix;
}

void Transform::_NotifyTransformation()
{
	_UpdateLocalMatrix();
	_UpdateWolrdMatrix();
	_PropagateWorldMatrix();
}


void Transform::Translate(glm::vec3 translate) {
	this->m_position += m_right * translate.x + m_up * translate.y + m_front * translate.z;
	_NotifyTransformation();
}

void Transform::Scale(GLfloat scale) {
	this->m_scale *= scale;
}

void Transform::_UpdateRotationAxis() {
	glm::mat4 axis = glm::mat4(1.0f) * glm::yawPitchRoll(m_yaw, m_pitch, m_roll);
	m_front = axis[2]; m_right = axis[0]; m_up = axis[1];
	_NotifyTransformation();
}

void Transform::Rotate(GLfloat pitch, GLfloat yaw, GLfloat roll) {
	this->m_pitch += pitch;
	this->m_yaw += yaw;
	this->m_roll += roll;
	_UpdateRotationAxis();
}


glm::mat4 Transform::TransformMatrix(bool doScale) {
	if (!doScale) return m_worldMatrix;
	return m_worldMatrix * glm::scale(glm::mat4(), glm::vec3(this->m_scale, this->m_scale, this->m_scale));
}


Transform::~Transform()
{
	for (size_t i = 0; i < m_updatables.size(); i++) {
		delete m_updatables[i];
	}
	for (size_t i = 0; i < m_children.size(); i++) {
		delete m_children[i];
	}
}
