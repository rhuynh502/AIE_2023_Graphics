#include "StationaryCamera.h"
#include "glm/ext.hpp"
#include "imgui.h"
#include "Gizmos.h"

StationaryCamera::StationaryCamera()
{
	m_rotationSpeed = 0;
}

void StationaryCamera::Draw()
{
	if (isMainCamera)
		return;

	float thetaR = glm::radians(m_theta);
	float phiR = glm::radians(m_phi + 90);

	glm::vec3 forward(glm::cos(phiR) * glm::cos(thetaR), glm::sin(phiR),
		glm::cos(phiR) * glm::sin(thetaR));

	aie::Gizmos::addCylinderFilled(m_position, 0.5f, 0.5f, 10,
		glm::vec4(forward, 0));
}

void StationaryCamera::ImGui()
{
	ImGui::DragFloat3("Cam Position", &m_position[0], 0.05f);
	ImGui::DragFloat("Theta Rotation", &m_theta, 0.05f);
	ImGui::DragFloat("Phi Rotation", &m_phi, 0.05f);
}

void StationaryCamera::SetRotation(glm::vec3 rotation)
{
	m_theta = rotation.x;
	m_phi = rotation.y;
}
