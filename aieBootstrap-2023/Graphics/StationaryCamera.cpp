#include "StationaryCamera.h"
#include "glm/ext.hpp"
#include "imgui.h"
#include "Gizmos.h"

StationaryCamera::StationaryCamera()
{
	m_rotationSpeed = 0;
}

void StationaryCamera::ImGui()
{
	ImGui::DragFloat3("Cam Position", &GetPosition()[0], 0.05f);
	ImGui::DragFloat("Theta Rotation", GetTheta(), 0.05f);
	ImGui::DragFloat("Phi Rotation", GetPhi(), 0.05f);
}

void StationaryCamera::SetRotation(glm::vec3 rotation)
{
	m_theta = rotation.x;
	m_phi = rotation.y;
}
