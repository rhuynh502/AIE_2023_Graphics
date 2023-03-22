#include "SimpleCamera.h"
#include <glm/ext.hpp>
#include "Input.h"

SimpleCamera::SimpleCamera()
{
	m_position = glm::vec3(-10.f, 2.f, 0.f);
	m_phi = 0;
	m_theta = 0;

}

void SimpleCamera::Update(float deltaTime)
{
	
}

glm::mat4 SimpleCamera::GetViewMatrix()
{
	float thetaR = glm::radians(m_theta);
	float phiR = glm::radians(m_phi);
	glm::vec3 forward(glm::cos(phiR) * glm::cos(thetaR), glm::sin(phiR),
		glm::cos(phiR) * glm::sin(thetaR));

	return glm::lookAt(m_position, m_position + forward, glm::vec3(0, 1, 0));
}

glm::mat4 SimpleCamera::GetProjectionMatrix(float width, float height)
{
	return glm::perspective(glm::pi<float>() * 0.25f, width / height,
		0.1f, 1000.f);
}

glm::mat4 SimpleCamera::GetWorldTransform(glm::vec3 position, glm::vec3 eulerAngles, glm::vec3 scale)
{
	return glm::mat4();
}

void SimpleCamera::SetPosition(glm::vec3 position)
{
	m_position = position;
}

void SimpleCamera::SetAspectRatio(float width, float height)
{
	m_aspectRatio = width / height;
}

void SimpleCamera::SetViewMatrix(glm::vec3 from, glm::vec3 to, glm::vec3 up)
{
	m_viewTransform = glm::lookAt(from, to, up);
}

void SimpleCamera::SetProjectionMatrix(float fieldOfView, float aspectRatio, float, float)
{
	m_projectionViewTransform = glm::perspective(glm::pi<float>() * 0.25f, aspectRatio,
		0.1f, 1000.f);
}

void SimpleCamera::SetProjectionMatrix(float fieldOfView, float width, float height, float, float)
{
	m_projectionViewTransform = glm::perspective(glm::pi<float>() * 0.25f, width / height,
		0.1f, 1000.f);
}
