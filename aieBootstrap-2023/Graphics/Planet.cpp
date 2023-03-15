#include "Planet.h"
#include "Gizmos.h"
#include <glm/ext.hpp>

Planet::Planet(glm::mat4 _matrix, float _distFromSun, glm::vec4 _color, float _rotationSpeed, float _radius, bool _hasRing)
{
	m_matrix = _matrix;
	m_distFromSun = _distFromSun;
	m_color = _color;
	m_rotationSpeed = _rotationSpeed;
	m_radius = _radius;
	m_hasRing = _hasRing;
}

Planet::~Planet()
{
}

void Planet::Update(float _deltaTime)
{
	m_rotation += m_rotationSpeed * _deltaTime;

	if (m_parentPlanet)
		m_matrix[3] = glm::translate(glm::translate(glm::mat4(1), m_parentPlanet->GetPosition()),
			glm::vec3(sin(m_rotation) * m_distFromSun, m_axis * sin(m_rotation) * m_distFromSun, cos(m_rotation) * m_distFromSun))[3];

	m_matrix = glm::rotate(m_matrix, _deltaTime * 0.2f, glm::vec3(0, 1, 0));
}

void Planet::Draw()
{
	aie::Gizmos::addSphere(GetPosition(), m_radius, 8, 8, m_color, &m_matrix);
	if (m_hasRing)
		aie::Gizmos::addRing(GetPosition(), m_radius + 0.1f, m_radius + 0.2f, 8, m_color, &m_matrix);
}

glm::vec3 Planet::GetPosition()
{
	return glm::vec3(m_matrix[3][0], m_matrix[3][1], m_matrix[3][2]);
}
