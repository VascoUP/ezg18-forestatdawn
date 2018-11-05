#pragma once

#include <stdio.h>
#include <vector>
#include <algorithm>

#include <GL\glew.h>
#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Updatable.h"

class Transform : public Updatable
{
private:
	static int NEXT_ID;
	const int OBJECT_ID = NEXT_ID++;

	//! Parent transform. Can be null
	Transform* parent;


	bool active = true;
	std::vector<Updatable*> updatables;

	//! List of all children.
	std::vector<Transform*> children;

	//!	Transformation matrix from individual coordinates to model coordinates
	glm::mat4 localTransformation;
	//!	Transformation matrix from model coordinates to world coordinates
	glm::mat4 worldTransformation;

	//!	Local position of the object
	glm::vec3 position;
	//!	X is pitch, y is yaw and z is roll
	glm::vec3 rotation;
	//!	Scale for each axis
	glm::vec3 scale;

	//!	Front direction vector 
	glm::vec3 front;
	//!	Right direction vector
	glm::vec3 right;
	//!	Up direction vector
	glm::vec3 up;

	/*!
		\fn void Transform::UpdateRotationAxis()

		Updates the front, right and up direction vector according to the rotation vector

		This function is called when the rotation vector is updated on the Rotate function.
	*/
	void UpdateRotationAxis();

	/*!
		\n void Transform::InitializeVariables()

		Initializes all variables to their default values
	*/
	void InitializeVariables();

	/*!
		\n glm::mat4 Transform::CalculateRotationMatrix()
	
		Calculates the rotation matrix according to the pitch, yaw and roll 
	*/
	glm::mat3 CalculateRotationMatrix();

public:
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


	glm::vec3 GetPosition() const;

	glm::vec3 GetFront() const;

	glm::vec3 GetUp() const;
	
	std::vector<Transform*> GetChildren();

	void AddChild(Transform* child);
	

	void SetUp();

	void Update();

	void SetActive(bool active);

	void AddUpdatable(Updatable* updatable);


	/*!
		\n void Transform::Translate(glm::vec3 translate)
		\param glm::vec3 translate Displacement to be applied to this object

		Translates this object by a given displacement
	*/
	void Translate(glm::vec3 translate);
	/*!
		\n void Transform::Scale(glm::vec3 scale)
		\param glm::scale Scale amount for each axis

		Scales the object by a given amount
	*/
	void Scale(glm::vec3 scale);
	/*!
		\n void Transform::Rotate(float angle, glm::vec3 axis)
		\param glm::vec3 axis The angles for each axis

		Rotates the object arount itself by the given angles
	*/
	void Rotate(glm::vec3 axis);

	/*!
		\n glm::mat4 Transform::GetWorldTransform()

		Returns the world trasnformation matrix.
		Assumes the matrix is updated, so no operations are computed.
	*/
	glm::mat4 GetWorldTransform();

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

