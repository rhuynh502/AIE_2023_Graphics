#include "Planet.h"
#include "Gizmos.h"
#include "imgui.h"
#include <glm/ext.hpp>
#include <string>

Planet::Planet(glm::mat4 _matrix, float _distFromSun, glm::vec4 _color, float _rotationSpeed, float _radius, const char* planetName, bool _hasRing)
{
	m_matrix = _matrix;
	m_distFromSun = _distFromSun;
	m_color = _color;
	m_rotationSpeed = _rotationSpeed;
	m_radius = _radius;
	m_hasRing = _hasRing;
	m_planetName = planetName;
}

Planet::~Planet()
{
}

void Planet::Update(float _deltaTime)
{
	for (auto planet : m_childrenPlanets)
		planet->Update(_deltaTime);

	if (!planetOn)
		return;

	m_rotation += m_rotationSpeed * _deltaTime;

	if (m_parentPlanet)
		m_matrix[3] = glm::translate(glm::translate(glm::mat4(1), m_parentPlanet->GetPosition()),
			glm::vec3(sin(m_rotation) * m_distFromSun, m_axis * sin(m_rotation) * m_distFromSun, cos(m_rotation) * m_distFromSun))[3];

	m_matrix = glm::rotate(m_matrix, _deltaTime * 1.6f, glm::vec3(0, 1, 0));
	ImGui();
}

void Planet::Draw()
{
	for (auto planet : m_childrenPlanets)
		planet->Draw();

	if (!planetOn)
		return;
	aie::Gizmos::addSphere(GetPosition(), m_radius, 8, 8, m_color, &m_matrix);
	if (m_hasRing)
		aie::Gizmos::addRing(GetPosition(), m_radius + 0.1f, m_radius + 0.2f, 8, m_color, &m_matrix);
}

glm::vec3 Planet::GetPosition()
{
	return glm::vec3(m_matrix[3][0], m_matrix[3][1], m_matrix[3][2]);
}

void Planet::AddChild(Planet* _planet)
{
	Planet* parent = _planet->GetParentPlanet();
	if (parent != this && parent != nullptr)
	{
		// Remove itself as child from original parent
		parent->m_childrenPlanets.erase(std::remove(parent->m_childrenPlanets.begin(),
			m_childrenPlanets.end(), _planet), m_childrenPlanets.end());
	}
	_planet->SetParentPlanet(this);

}

void Planet::TogglePlanet()
{
	planetOn = !planetOn;
}

void Planet::ImGui()
{
	if (!planetOn)
		return;

	ImGui::Columns(2);
	std::string turnOff = "Turn Off ";
	if (ImGui::Button((turnOff + m_planetName).c_str()))
		planetOn = !planetOn;

	std::string name = "Speed";
	ImGui::DragFloat((m_planetName + name).c_str(),
		&m_rotationSpeed, 0.05f);
	std::string color = "Color";
	ImGui::DragFloat3((m_planetName + color).c_str(),
		&m_color[0], 0.05f, 0.f, 1.f);

	std::string children = "Children";
	ImGui::NextColumn();
	if (m_childrenPlanets.size() > 0 && 
		ImGui::CollapsingHeader((m_planetName + children).c_str()))
	{
		for (auto planet : m_childrenPlanets)
		{
			ImGui::Checkbox(planet->GetName(), &planet->planetOn);
		}
	}
}
