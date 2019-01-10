#pragma once

#include <GL\glew.h>

#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\type_ptr.hpp>

#include "AObjectBehavior.h"
#include "Transform.h"
#include "Time.h"
#include "Input.h"
#include "GLWindow.h"

class Camera: public AObjectBehavior
{
private:
	static Camera* camera;
	Camera(Transform* object, GLWindow* window);

	glm::mat4 projectionMatrix;
public:
	static Camera* CreateInstance(Transform* object, GLWindow* window);
	static Camera* GetInstance();

	glm::vec3 GetCameraPosition();

	glm::mat4 CalculateViewMatrix();
	glm::mat4 ProjectionMatrix();

	bool PointInsideViewFrustum(glm::vec3* point, float bias);
	static bool PointInsideViewFrustum(float cameraAngle, glm::vec3* cameraPosition, glm::vec3* cameraFront, glm::vec3* point, float bias);

	void SetUp();
	void Update();

	~Camera();
};