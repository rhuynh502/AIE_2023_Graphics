#pragma once
#include <glm/mat4x4.hpp>
#include <vector>
#include <string>

class Planet
{
public:
	Planet(glm::mat4 _matrix, float _distFromSun, glm::vec4 _color, float _rotationSpeed, float _radius, const char* planetName, bool _hasRing = false);
	~Planet();

	void Update(float _deltaTime);
	void Draw();

	void ImGui();

	// getters
	glm::vec3 GetPosition();
	glm::mat4 GetMatrix() { return m_matrix; }
	float GetDistFromSun() { return m_distFromSun; }
	Planet* GetParentPlanet() { return m_parentPlanet; }
	std::string GetName() { return m_planetName; }

	// setters
	void SetMatrix(glm::mat4 _matrix) { m_matrix = _matrix; }
	void SetParentPlanet(Planet* _planet) 
	{ 
		m_parentPlanet = _planet;
		_planet->m_childrenPlanets.push_back(this);
	}
	void SetAxis(float _axis) { m_axis = _axis; }
	void AddChild(Planet* _planet);

	void TogglePlanet();

	// Getters
	bool IsPlanetOn() { return m_planetOn; }
	bool* PlanetOn() { return &m_planetOn; }

	

protected:
	glm::mat4 m_matrix;
	float m_distFromSun;
	float m_rotationSpeed;
	float m_radius;
	Planet* m_parentPlanet = nullptr;
	std::vector<Planet*> m_childrenPlanets;
	glm::vec4 m_color;
	bool m_hasRing;
	float m_axis = 0;
	float m_rotation = 0;
	std::string m_planetName;

	bool m_planetOn = false;
};

