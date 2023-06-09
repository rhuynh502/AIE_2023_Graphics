#include "SimpleCamera.h"
#include <glm/ext.hpp>
#include "Gizmos.h"
#include "Input.h"
#include "Instance.h"
#include "imgui.h"

SimpleCamera::SimpleCamera()
{
}

void SimpleCamera::Update(float deltaTime)
{
	
}

void SimpleCamera::Draw()
{
	aie::Gizmos::addCylinderFilled(m_position, 0.5f, 0.5f, 10,
		{ 1, 1, 1, 0 });
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
	return glm::translate(glm::mat4(1), position)* glm::rotate(glm::mat4(1), glm::radians(eulerAngles.z), glm::vec3(0, 0, 1));
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

void SimpleCamera::ImGui()
{
	
}
