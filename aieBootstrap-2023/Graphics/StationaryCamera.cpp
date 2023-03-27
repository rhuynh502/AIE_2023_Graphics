#include "StationaryCamera.h"
#include "glm/ext.hpp"
#include "imgui.h"

StationaryCamera::StationaryCamera()
{
	m_rotationSpeed = 0;
}

void StationaryCamera::Update(float deltaTime)
{
	
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
