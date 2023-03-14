#pragma once

#include "Application.h"
#include <glm/mat4x4.hpp>
#include "Planet.h"
#include <vector>

class GraphicsApp : public aie::Application {
public:

	GraphicsApp();
	virtual ~GraphicsApp();

	virtual bool startup();
	virtual void shutdown();

	virtual void update(float deltaTime);
	virtual void draw();

	void InitialisePlanets();

protected:

	// camera transforms
	glm::mat4	m_viewMatrix;
	glm::mat4	m_projectionMatrix;

	std::vector<Planet*> m_planets;

	Planet* m_sun;
	Planet* m_mercury;
	Planet* m_venus;
	Planet* m_earth;
	Planet* m_mars;
	Planet* m_jupiter;
	Planet* m_saturn;
	Planet* m_uranus;

	Planet* m_moon;
};