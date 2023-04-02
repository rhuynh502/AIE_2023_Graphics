#pragma once
#include <glm/glm.hpp>

class SimpleCamera
{
public:
	SimpleCamera();
	~SimpleCamera() {};

	virtual void Update(float deltaTime);
	void Draw();
	glm::vec3 GetPosition() { return m_position; }
	glm::mat4 GetViewMatrix();
	glm::mat4 GetProjectionMatrix(float width, float height);
	glm::mat4 GetWorldTransform(glm::vec3 position, 
		glm::vec3 eulerAngles, glm::vec3 scale);
	float GetAspectRatio() { return m_aspectRatio; }
	float* GetRotationSpeed() { return &m_rotationSpeed; }

	void SetPosition(glm::vec3 position);
	void SetAspectRatio(float width, float height);
	void SetViewMatrix(glm::vec3 from, glm::vec3 to, glm::vec3 up);
	void SetProjectionMatrix(float fieldOfView, float aspectRatio, float near, float far);
	void SetProjectionMatrix(float fieldOfView, float width, float height, float near, float far);
	void SetRotationSpeed(float rotationSpeed) { m_rotationSpeed = rotationSpeed; }

	float* GetTheta() { return &m_theta; }
	float* GetPhi() { return& m_phi; }

	void ToggleCamera() { isMainCamera = !isMainCamera; }
	virtual void ImGui();

	bool isMainCamera = false;

protected:
	glm::mat4 m_projectionViewTransform;
	glm::mat4 m_worldTransform;
	glm::mat4 m_viewTransform;

	float m_aspectRatio;

	float m_theta;
	float m_phi;

	float m_rotationSpeed = glm::radians(180.f);

	glm::vec3 m_position;

	// Last position of mouse
	glm::vec2 m_lastMouse;

};

