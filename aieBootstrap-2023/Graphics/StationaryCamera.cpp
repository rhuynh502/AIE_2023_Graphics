#include "StationaryCamera.h"
#include "glm/ext.hpp"

void StationaryCamera::Update(float deltaTime)
{
	
}

void StationaryCamera::SetRotation(glm::vec3 rotation)
{
	m_theta = rotation.x;
	m_phi = rotation.y;
}
