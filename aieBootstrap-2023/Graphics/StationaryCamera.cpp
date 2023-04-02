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
	
}

void StationaryCamera::SetRotation(glm::vec3 rotation)
{
	m_theta = rotation.x;
	m_phi = rotation.y;
}
