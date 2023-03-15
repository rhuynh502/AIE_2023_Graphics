#pragma once
#include <glm/mat4x4.hpp>

class Planet
{
public:
	Planet(glm::mat4 _matrix, float _distFromSun, glm::vec4 _color, float _rotationSpeed, float _radius, bool _hasRing = false);
	~Planet();

	void Update(float _deltaTime);
	void Draw();

	// getters
	glm::vec3 GetPosition();
	glm::mat4 GetMatrix() { return m_matrix; }
	float GetDistFromSun() { return m_distFromSun; }

	// setters
	void SetMatrix(glm::mat4 _matrix) { m_matrix = _matrix; }
	void SetParentPlanet(Planet* _planet) { m_parentPlanet = _planet; }
	void SetAxis(float _axis) { m_axis = _axis; }


protected:
	glm::mat4 m_matrix;
	float m_distFromSun;
	float m_rotationSpeed;
	float m_radius;
	Planet* m_parentPlanet = nullptr;
	glm::vec4 m_color;
	bool m_hasRing;
	float m_axis = 0;
	float m_rotation = 0;
};

