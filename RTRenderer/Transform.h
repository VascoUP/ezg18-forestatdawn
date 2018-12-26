#pragma once

#include <stdio.h>
#include <vector>
#include <algorithm>

#include <GL\glew.h>
#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\type_ptr.hpp>
#include <glm\gtx\quaternion.hpp>
#include <glm/gtx/euler_angles.hpp>

#include "IUpdatable.h"

class Transform : public IUpdatable
{
private:
	static int NEXT_ID;
	const int OBJECT_ID = NEXT_ID++;

	bool m_static = true;

	//! List of all object behaviours
	std::vector<IUpdatable*> m_updatables;
	//! List of all children.
	std::vector<Transform*> m_children;

	//!	Transformation matrix from individual coordinates to model coordinates
	glm::mat4 m_localMatrix;
	//!	Transformation matrix from model coordinates to world coordinates
	glm::mat4 m_worldMatrix;

	//!	Local position of the object
	glm::vec3 m_position;
	//!	X is pitch, y is yaw and z is roll
	GLfloat m_pitch;
	GLfloat m_yaw;
	GLfloat m_roll;
	//!	Scale for each axis
	GLfloat m_scale;

	//!	Front direction vector 
	glm::vec3 m_front;
	//!	Right direction vector
	glm::vec3 m_right;
	//!	Up direction vector
	glm::vec3 m_up;

	//! Quaternion
	glm::quat m_quaternion;

	void _NotifyTransformation();

	/*!
		\n void Transform::UpdateRotationAxis()

		Updates the front, right and up direction vector according to the rotation vector

		This function is called when the rotation vector is updated on the Rotate function.
	*/
	void _UpdateRotationAxis();

	/*!
		\n void UpdateLocalMatrix()

		Updates the local matrix so that at any given time that variable is consistent with the actual transformation
	*/
	void _UpdateLocalMatrix();

	void _UpdateWolrdMatrix();

	void _PropagateWorldMatrix();

	/*!
		\n void PropagateWorldMatrix(glm::mat4 parentWorldMatrix)

		Updates the world matrix of itself and propagates it to it's children
	*/
	void _PropagateWorldMatrix(glm::mat4 parentWorldMatrix);

	/*!
		\n void Transform::InitializeVariables()

		Initializes all variables to their default values
	*/
	void _InitializeVariables();


	static void _SetChildrenDynamic(Transform* transform);

	static bool _GetParentsAreStatic(Transform* transform);

public:
	//! Parent transform. Can be null
	Transform* parent;

	enum POINT_TYPE {
		DIRECTION, POINT
	};

	/*!
		\n Transform::Transform()

		Constructor: initializes all variables to their default values
	*/
	Transform();
	/*!
		\n Transform::Transform(Transform* parent)
		\param Transform* parent Scene parent of this object

		Constructor: initializes all variables to their default value, except parent which is set to the value passed as a parameter
	*/
	Transform(Transform* parent);
	

	bool SetStatic(bool isStatic);

	bool GetStatic() const;

	glm::vec3 GetPosition();

	glm::vec3 GetRotation() const;

	glm::vec3 GetWorldPosition();

	glm::vec3 GetFront();

	glm::vec3 GetUp();

	glm::vec3 GetWorldUp();
	
	glm::mat4 GetWorldMatrix() const;

	glm::mat4 GetLocalMatrix() const;

	glm::vec3 LocalToWorldCoordinates(glm::vec3 point, POINT_TYPE type);

	std::vector<Transform*> GetChildren();

	void AddChild(Transform* child);
	

	void SetUp();

	void Update();
	
	void AddUpdatable(IUpdatable* updatable);


	/*!
		\n void Transform::Translate(glm::vec3 translate)
		\param glm::vec3 translate Displacement to be applied to this object

		Translates this object by a given displacement
	*/
	void Translate(glm::vec3 translate);
	/*!
		\n void Transform::Scale(glm::vec3 scale)
		\param GLfloat scale Scale amount for each axis

		Scales the object by a given amount
	*/
	void Scale(GLfloat scale);
	/*!
		\n void Transform::Rotate(float angle, glm::vec3 axis)
		\param glm::vec3 axis The angles for each axis

		Rotates the object arount itself by the given angles
	*/
	void Rotate(GLfloat pitch, GLfloat yaw, GLfloat roll);
	
	/*!
		\n glm::mat4 Transform::TransformMatrix(bool doScale)
		\param bool doScale Tells the function if it performes the scale operation or not

		Returns the world transformation matrix which is a result of the model transformation matrix multiplied by the parent's world transformation matrix
	*/
	glm::mat4 TransformMatrix(bool doScale);

	/*!
		\n Transform::~Transform()

		Destructor
	*/
	~Transform();
};

