#include "GraphicsApp.h"
#include "Gizmos.h"
#include "Input.h"
#include <glm/glm.hpp>
#include <glm/ext.hpp>

using glm::vec3;
using glm::vec4;
using glm::mat4;
using aie::Gizmos;

GraphicsApp::GraphicsApp() {

}

GraphicsApp::~GraphicsApp() {

}

bool GraphicsApp::startup() {
	
	setBackgroundColour(0.25f, 0.25f, 0.25f);

	// initialise gizmo primitive counts
	Gizmos::create(10000, 10000, 10000, 10000);

	// create simple camera transforms
	m_viewMatrix = glm::lookAt(vec3(10), vec3(0), vec3(0, 1, 0));
	m_projectionMatrix = glm::perspective(glm::pi<float>() * 0.25f, 16.0f / 9.0f, 0.1f, 1000.0f);

	//InitialisePlanets();

	return true;
}

void GraphicsApp::shutdown() {

	Gizmos::destroy();
}

void GraphicsApp::update(float deltaTime) {

	float time = getTime();

	// wipe the gizmos clean for this frame
	Gizmos::clear();

	// draw a simple grid with gizmos
	vec4 white(1);
	vec4 black(0, 0, 0, 1);
	for (int i = 0; i < 21; ++i) {
		Gizmos::addLine(vec3(-10 + i, 0, 10),
						vec3(-10 + i, 0, -10),
						i == 10 ? white : black);
		Gizmos::addLine(vec3(10, 0, -10 + i),
						vec3(-10, 0, -10 + i),
						i == 10 ? white : black);
	}

	// add a transform so that we can see the axis
	Gizmos::addTransform(mat4(1));

	for (auto planet : m_planets)
		planet->Update(deltaTime, time);

	// quit if we press escape
	aie::Input* input = aie::Input::getInstance();

	if (input->isKeyDown(aie::INPUT_KEY_ESCAPE))
		quit();

	if (input->isKeyDown(aie::INPUT_KEY_UP))
		m_sun->SetMatrix(glm::translate(m_sun->GetMatrix(), vec3(-0.1f, 0, 0)));
	if (input->isKeyDown(aie::INPUT_KEY_DOWN))
		m_sun->SetMatrix(glm::translate(m_sun->GetMatrix(), vec3(0.1f, 0, 0)));
	if (input->isKeyDown(aie::INPUT_KEY_LEFT))
		m_sun->SetMatrix(glm::translate(m_sun->GetMatrix(), vec3(0, 0, 0.1f)));
	if (input->isKeyDown(aie::INPUT_KEY_RIGHT))
		m_sun->SetMatrix(glm::translate(m_sun->GetMatrix(), vec3(0, 0, -0.1f)));
	

}

void GraphicsApp::draw() {

	// wipe the screen to the background colour
	clearScreen();

	// update perspective based on screen size
	m_projectionMatrix = glm::perspective(glm::pi<float>() * 0.25f, getWindowWidth() / (float)getWindowHeight(), 0.1f, 1000.0f);

	for(auto planet : m_planets)
		planet->Draw();

	Gizmos::draw(m_projectionMatrix * m_viewMatrix);
}

void GraphicsApp::InitialisePlanets()
{
	// initialise planets
	m_sun = new Planet(mat4(1), 0, vec4(1, 0, 0, 1), 0, 1);
	m_sun->SetMatrix(glm::translate(m_sun->GetMatrix(), vec3(0, 0, 0)));
	m_planets.push_back(m_sun);

	m_mercury = new Planet(glm::translate(m_sun->GetMatrix(), vec3(1.2f, 0, 0)), 1.2f, vec4(0.5f, 0.5f, 0, 1), 0.8f, 0.2f);
	m_mercury->SetParentPlanet(m_sun);
	m_mercury->SetAxis(1);
	m_planets.push_back(m_mercury);

	m_venus = new Planet(glm::translate(m_sun->GetMatrix(), vec3(1.6f, 0, 0)), 1.6f, vec4(1, 0.647f, 0, 1), 0.8f, 0.2f);
	m_venus->SetParentPlanet(m_sun);
	m_planets.push_back(m_venus);

	m_earth = new Planet(glm::translate(m_sun->GetMatrix(), vec3(2.f, 0, 0)), 2.f, vec4(0.2f, 0.2f, 1, 1), 0.6f, 0.2f);
	m_earth->SetParentPlanet(m_sun);
	m_planets.push_back(m_earth);

	m_mars = new Planet(glm::translate(m_sun->GetMatrix(), vec3(2.4f, 0, 0)), 2.4f, vec4(1, 0.7f, 0, 1), 0.7f, 0.2f);
	m_mars->SetParentPlanet(m_sun);
	m_planets.push_back(m_mars);

	m_jupiter = new Planet(glm::translate(m_sun->GetMatrix(), vec3(2.8f, 0, 0)), 2.8f, vec4(1, 0.87f, 0, 1), 0.45f, 0.2f, true);
	m_jupiter->SetParentPlanet(m_sun);
	m_planets.push_back(m_jupiter);

	m_saturn = new Planet(glm::translate(m_sun->GetMatrix(), vec3(3.2f, 0, 0)), 3.2f, vec4(1, 1, 1, 1), 0.4f, 0.2f, true);
	m_saturn->SetParentPlanet(m_sun);
	m_planets.push_back(m_saturn);

	m_uranus = new Planet(glm::translate(m_sun->GetMatrix(), vec3(3.6f, 0, 0)), 3.6f, vec4(0, 0, 1, 1), 0.2f, 0.2f);
	m_uranus->SetParentPlanet(m_sun);
	m_planets.push_back(m_uranus);

	m_moon = new Planet(glm::translate(m_earth->GetMatrix(), vec3(0.2f, 0, 0)), 0.2f, vec4(1, 1, 1, 1), 1.5f, 0.05f);
	m_moon->SetParentPlanet(m_earth);
	m_moon->SetAxis(0.6f);
	m_planets.push_back(m_moon);
}
