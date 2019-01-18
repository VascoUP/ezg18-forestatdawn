#include "ObjectController.h"


glm::vec3 Interpolate(glm::vec3 v1, glm::vec3 v2, float percentage) {
	return v1 * (1.0f - percentage) + v2 * percentage;
}

CameraController::CameraController(Transform* container, GLfloat moveSpeed, GLfloat turnSpeed)
	: AObjectBehavior(container)
{
	this->moveSpeed = moveSpeed;
	this->turnSpeed = turnSpeed;
}

void CameraController::SetUp() {
	// Make sure that this scene object has a Camera
}

void CameraController::KeyControl() {
	GLfloat velocity = moveSpeed * (GLfloat)Time::GetDeltaTime();
	if (Input::IsKeyPress(GLFW_KEY_W)) {
		this->transform->Translate(glm::vec3(0.0f, 0.0f, velocity));
	}
	if (Input::IsKeyPress(GLFW_KEY_S)) {
		this->transform->Translate(glm::vec3(0.0f, 0.0f, -velocity));
	}
	if (Input::IsKeyPress(GLFW_KEY_D)) {
		this->transform->Translate(glm::vec3(-velocity, 0.0f, 0.0f));
	}
	if (Input::IsKeyPress(GLFW_KEY_A)) {
		this->transform->Translate(glm::vec3(velocity, 0.0f, 0.0f));
	}
}

void CameraController::MouseControl() {
	GLfloat xDelta, yDelta;
	Input::GetMouseDelta(&xDelta, &yDelta);

	xDelta *= turnSpeed * (GLfloat)Time::GetDeltaTime();
	yDelta *= turnSpeed * (GLfloat)Time::GetDeltaTime();

	this->transform->Rotate(-yDelta, -xDelta, 0.0f);
}

void CameraController::Update() {
	KeyControl();
	MouseControl();
}

AnimateKeyFrame::AnimateKeyFrame(Transform * container, std::vector<KeyFrame>* keyFrames)
	: AObjectBehavior(container)
{
	m_keyframes = *keyFrames;
}

void AnimateKeyFrame::SetUp()
{
	m_index = 0;
	m_deltaTime = 0;

	if (m_keyframes.size() > 0) {
		// Go to first key frame
		{
			glm::vec3 deltaTranslate = m_keyframes[0].position - transform->GetPosition();
			transform->TranslateLocal(deltaTranslate);
		}
		{
			glm::vec3 deltaRotation = m_keyframes[0].rotation - transform->GetRotation();
			transform->Rotate(deltaRotation.x, deltaRotation.y, deltaRotation.z);
		}
	}
}

void AnimateKeyFrame::Update()
{
	// End of behavior
	if (m_index + 1 >= m_keyframes.size())
		return;

	m_deltaTime += (GLfloat)Time::GetDeltaTime();

	float residue;
	do {
		residue = 0.0f;
		float percentage = m_deltaTime / m_keyframes[m_index + 1].deltaTime;
		if (percentage > 1.0f)
		{
			residue = percentage - 1.0f;
			percentage = 1.0f;
		}
		
		{
			// Go to position
			glm::vec3 position = Interpolate(m_keyframes[m_index].position, m_keyframes[m_index + 1].position, percentage);
			glm::vec3 deltaTranslate = position - transform->GetPosition();
			transform->TranslateLocal(deltaTranslate);
		}
		{
			// Go to rotation
			glm::vec3 rotation = Interpolate(m_keyframes[m_index].rotation, m_keyframes[m_index + 1].rotation, percentage);
			glm::vec3 deltaRotation = rotation - transform->GetRotation();
			transform->Rotate(deltaRotation.x, deltaRotation.y, deltaRotation.z);
		}

		if (residue > 0.0f) {
			m_index++;
			m_deltaTime = m_deltaTime - m_keyframes[m_index].deltaTime;
		}
	} while (residue > 0.0f && m_index >= m_keyframes.size());

}

PrintKeyFrame::PrintKeyFrame(Transform * container) :
	AObjectBehavior(container)
{}

void PrintKeyFrame::SetUp()
{
	m_wasPressed = false;
}

void PrintKeyFrame::Update()
{
	if (!m_wasPressed && Input::IsKeyPress(GLFW_KEY_SPACE)) {
		m_wasPressed = true;
		printf("kf = new KeyFrame(); kf->deltaTime = 0.0f;\nkf->position = glm::vec3(%ff, %ff, %ff); kf->rotation = glm::vec3(%ff, %ff, %ff);\ncameraKeyFrames->push_back(*kf);\n", 
			transform->GetPosition().x, transform->GetPosition().y, transform->GetPosition().z,
			transform->GetRotation().x, transform->GetRotation().y, transform->GetRotation().z);
	}
	else if (m_wasPressed && !Input::IsKeyPress(GLFW_KEY_SPACE)) {
		m_wasPressed = false;
	}
}

ActivateLights::ActivateLights(Transform* transform) :
	AObjectBehavior(transform)
{}

void ActivateLights::SetUp() {
	for (size_t i = 0; i < 9; i++) {
		m_wasPressed[i] = false;
	}
}

void ActivateLights::Update() {
	for (size_t i = 0; i < 9; i++) {
		if (!m_wasPressed[i] && Input::IsKeyPress(GLFW_KEY_1 + i)) {
			m_wasPressed[i] = true;
			Light* light = GLProgram::GetGLRenderer()->GetPointLightAt(i);
			if(light != nullptr)
				light->SetActive(!light->IsActive());
		} 
		else if (m_wasPressed && !Input::IsKeyPress(GLFW_KEY_1 + i)) {
			m_wasPressed[i] = false;
		}
	}
}

HelicopterController::HelicopterController(Transform* container, float vel, float rotVel)
	: AObjectBehavior(container)
{
	moveSpeed = vel;
	rotSpeed = rotVel;
}

void HelicopterController::SetUp() {

}

void HelicopterController::Update() {
	{
		GLfloat velocity = moveSpeed * (GLfloat)Time::GetDeltaTime();
		this->transform->Translate(glm::vec3(0.0f, velocity, 0.0f));
	}
	{
		GLfloat angularVelocity = rotSpeed * (GLfloat)Time::GetDeltaTime();
		this->transform->Rotate(0.0f, angularVelocity, 0.0f);
	}
}
 