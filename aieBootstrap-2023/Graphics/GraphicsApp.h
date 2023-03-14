#pragma once

#include "Application.h"
#include <glm/mat4x4.hpp>
#include "Planet.h"
#include "Mesh.h"
#include "Shader.h"
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

	bool LaunchShaders();

	// camera transforms
	glm::mat4	m_viewMatrix;
	glm::mat4	m_projectionMatrix;

	aie::ShaderProgram m_simpleShader;
	Mesh m_quadMesh;
	glm::mat4 m_quadTransform;

	std::vector<Planet*> m_planets;

	Planet* m_sun;
};