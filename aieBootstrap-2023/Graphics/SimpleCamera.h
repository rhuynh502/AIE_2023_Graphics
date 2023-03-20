#pragma once
#include <glm/glm.hpp>

class SimpleCamera
{
public:
	SimpleCamera();
	~SimpleCamera() {};

	void Update(float deltaTime);
	glm::vec3 GetPosition() { return m_position; }
	glm::mat4 GetViewMatrix();
	glm::mat4 GetProjectionMatrix(float width, float height);
	glm::mat4 GetTransform(glm::vec3 position, 
		glm::vec3 eulerAngles, glm::vec3 scale);

protected:
	float m_theta;
	float m_phi;

	float m_rotationSpeed = glm::radians(180.f);

	glm::vec3 m_position;

	// Last position of mouse
	glm::vec2 m_lastMouse;
	
};

