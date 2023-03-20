#include "GraphicsApp.h"
#include "Gizmos.h"
#include "Input.h"
#include <imgui.h>
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

	m_light.color = { 1, 1, 0 };
	m_ambientLight = { 0.5, 0.5, 0.5 };

	InitialisePlanets();

	return LaunchShaders();
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

	if(m_planetsOn)
		for (auto planet : m_planets)
			planet->Update(deltaTime);

	// quit if we press escape
	aie::Input* input = aie::Input::getInstance();

	// Rotate the light to emulate a 'day/night' cycle
	m_light.direction = glm::normalize(glm::vec3(glm::cos(time * 2), glm::sin(time * 2), 0));

	m_camera.Update(deltaTime);

	if (input->isKeyDown(aie::INPUT_KEY_ESCAPE))
		quit();

	ImGUIRefresher();
}

void GraphicsApp::draw() {

	// wipe the screen to the background colour
	clearScreen();

	// update perspective based on screen size
	// create simple camera transforms
	m_viewMatrix = m_camera.GetViewMatrix();
	m_projectionMatrix = m_camera.GetProjectionMatrix((float)getWindowWidth(),
		(float)getWindowHeight());

	// Draw the quad set up in QuadLoader()
	auto pvm = m_projectionMatrix * m_viewMatrix;

	if(m_cubeOn)
		CubeDraw(pvm * m_cubeTransform);

	// Draw the bunny set up in BunnyLoader()
		//BunnyDraw(pvm * m_bunnyTransform);

	// Draw the light
	if (m_bunnyOn)
	{
		PhongDraw(pvm * m_bunnyTransform, m_bunnyTransform);
		SpearDraw(pvm * m_spearTransform, m_spearTransform);
	}

	CylinderDraw(pvm * m_cylinderTransform);

	if(m_planetsOn)
		for(auto planet : m_planets)
			planet->Draw();

	Gizmos::draw(m_projectionMatrix * m_viewMatrix);
}

void GraphicsApp::InitialisePlanets()
{
	// initialise planets
	m_sun = new Planet(mat4(1), 0, vec4(1, 0, 0, 1), 0, 0.1f);
	m_sun->SetMatrix(glm::translate(m_sun->GetMatrix(), vec3(0, 1, 0)));
	m_planets.push_back(m_sun);

	Planet* m_mercury = new Planet(glm::translate(m_sun->GetMatrix(), vec3(1.2f, 0, 0)), 1.2f, vec4(0.5f, 0.5f, 0, 1), 1.2f, 0.2f);
	m_mercury->SetParentPlanet(m_sun);
	m_mercury->SetAxis(-1);
	m_planets.push_back(m_mercury);

	Planet* m_venus = new Planet(glm::translate(m_sun->GetMatrix(), vec3(1.6f, 0, 0)), 1.6f, vec4(1, 0.647f, 0, 1), 0.8f, 0.2f);
	m_venus->SetParentPlanet(m_sun);
	m_venus->SetAxis(-1.2f);
	m_planets.push_back(m_venus);

	Planet* m_earth = new Planet(glm::translate(m_sun->GetMatrix(), vec3(2.f, 0, 0)), 2.f, vec4(0.2f, 0.2f, 1, 1), 0.6f, 0.2f);
	m_earth->SetParentPlanet(m_sun);
	m_planets.push_back(m_earth);

	Planet* m_mars = new Planet(glm::translate(m_sun->GetMatrix(), vec3(2.4f, 0, 0)), 2.4f, vec4(1, 0.7f, 0, 1), 0.7f, 0.2f);
	m_mars->SetParentPlanet(m_sun);
	m_mars->SetAxis(-0.64f);
	m_planets.push_back(m_mars);

	Planet* m_jupiter = new Planet(glm::translate(m_sun->GetMatrix(), vec3(2.8f, 0, 0)), 2.8f, vec4(1, 0.87f, 0, 1), 0.45f, 0.2f, true);
	m_jupiter->SetParentPlanet(m_sun);
	m_jupiter->SetAxis(0.77f);
	m_planets.push_back(m_jupiter);

	Planet* m_saturn = new Planet(glm::translate(m_sun->GetMatrix(), vec3(3.2f, 0, 0)), 3.2f, vec4(1, 1, 1, 1), 0.4f, 0.2f, true);
	m_saturn->SetParentPlanet(m_sun);
	m_saturn->SetAxis(-0.2f);
	m_planets.push_back(m_saturn);

	Planet* m_uranus = new Planet(glm::translate(m_sun->GetMatrix(), vec3(3.6f, 0, 0)), 3.6f, vec4(0, 0, 1, 1), 0.2f, 0.2f);
	m_uranus->SetParentPlanet(m_sun);
	m_uranus->SetAxis(0.466f);
	m_planets.push_back(m_uranus);

	Planet* m_moon = new Planet(glm::translate(m_earth->GetMatrix(), vec3(0.2f, 0, 0)), 0.2f, vec4(1, 1, 1, 1), 1.5f, 0.05f);
	m_moon->SetParentPlanet(m_earth);
	m_moon->SetAxis(0.6f);
	m_planets.push_back(m_moon);
}

bool GraphicsApp::LaunchShaders()
{
	// Used to load quad
	if (!QuadLoader())
		return false;

	// Used to load cube
	if (!CubeLoader())
		return false;

	// Used to load OBJ bunny
	if (!BunnyLoader())
		return false;

	// Used to load OBJ spear
	if (!SpearLoader())
		return false;

	if (!CylinderLoader(3, 3, 3))
		return false;

	return true;
}

void GraphicsApp::ImGUIRefresher()
{
	ImGui::Begin("Light Settings");
	ImGui::DragFloat3("Global Light Color", 
		&m_light.color[0], 0.1, 0, 1);
	ImGui::DragFloat3("Global Light Direction",
		&m_light.direction[0], 0.1, -1, 1);
	ImGui::End();

	ImGui::Begin("Planets Settings");
	if(ImGui::Button(m_planetsOn ? "DEACTIVATE BUNNY POWERS" : "ACTIVATE BUNNY POWERS"))
		m_planetsOn = !m_planetsOn;
	if(ImGui::Button(m_bunnyOn ? "DEACTIVATE BUNNY" : "ACTIVATE BUNNY"))
		m_bunnyOn = !m_bunnyOn;
	ImGui::End();

	ImGui::Begin("Shapes Settings");
	if (ImGui::Button(m_cubeOn ? "Cube off" : "Cube On"))
		m_cubeOn = !m_cubeOn;
	ImGui::DragFloat3("Position",
		&m_cubeTransform[3][0]);
	ImGui::DragFloat3("Position Cylinder",
		&m_cylinderTransform[3][0]);
	ImGui::End();
}

bool GraphicsApp::QuadLoader()
{
	m_simpleShader.loadShader(aie::eShaderStage::VERTEX,
		"./shaders/simple.vert");
	m_simpleShader.loadShader(aie::eShaderStage::FRAGMENT,
		"./shaders/simple.frag");

	if (!m_simpleShader.link())
	{
		printf("Simple Shader has an Error: %s\n",
			m_simpleShader.getLastError());
		return false;
	}

	// Defined as 4 vertices for the 2 triangles
	Mesh::Vertex vertices[4];
	vertices[0].position = { -0.5f, 0,  0.5f, 1 };
	vertices[1].position = { 0.5f, 0,  0.5f, 1 };
	vertices[2].position = { -0.5f, 0, -0.5f, 1 };
	vertices[3].position = { 0.5f, 0, -0.5f, 1 };

	unsigned int indices[6] = { 0, 1, 2, 2, 1, 3 };

	m_quadMesh.Initialise(4, vertices, 6, indices);

	//This is a 10 'unit' wide quad
	m_quadTransform =
	{
		10, 0, 0, 0,
		0, 10, 0, 0,
		0, 0, 10, 0,
		0, 0, 0, 1
	};

	return true;

}

void GraphicsApp::QuadDraw(glm::mat4 pvm)
{
	// Bind the shader
	m_simpleShader.bind();

	// Bind the transform
	
	m_simpleShader.bindUniform("ProjectionViewModel", pvm);

	// Draw the quad using Mesh's draw
	m_quadMesh.Draw();
}

bool GraphicsApp::CubeLoader()
{
	m_simpleShader.loadShader(aie::eShaderStage::VERTEX,
		"./shaders/simple.vert");
	m_simpleShader.loadShader(aie::eShaderStage::FRAGMENT,
		"./shaders/simple.frag");

	if (!m_simpleShader.link())
	{
		printf("Simple Shader has an Error: %s\n",
			m_simpleShader.getLastError());
		return false;
	}

	// Defined as 4 vertices for the 2 triangles
	Mesh::Vertex vertices[8];

	glm::vec4 center = { 0, 0, 0, 0 };
	float distFromCenter = 1;

	vertices[0].position = { center.x + distFromCenter, center.y + distFromCenter, center.z + distFromCenter, 1 };
	vertices[1].position = { center.x + distFromCenter, center.y - distFromCenter, center.z + distFromCenter, 1 };
	vertices[2].position = { center.x - distFromCenter, center.y - distFromCenter, center.z + distFromCenter, 1 };
	vertices[3].position = { center.x - distFromCenter, center.y + distFromCenter, center.z + distFromCenter, 1 };
	vertices[4].position = { center.x + distFromCenter, center.y + distFromCenter, center.z - distFromCenter, 1 };
	vertices[5].position = { center.x + distFromCenter, center.y - distFromCenter, center.z - distFromCenter, 1 };
	vertices[6].position = { center.x - distFromCenter, center.y - distFromCenter, center.z - distFromCenter, 1 };
	vertices[7].position = { center.x - distFromCenter, center.y + distFromCenter, center.z - distFromCenter, 1 };

	unsigned int indices[36] = { 0, 2, 1, 2, 0, 3, 
								 4, 1, 5, 4, 0, 1,
								 7, 5, 6, 7, 4, 5,
								 3, 6, 2, 3, 7, 6,
								 1, 6, 5, 1, 2, 6,
								 4, 3, 0, 4, 7, 3 };

	m_cubeMesh.Initialise(8, vertices, 36, indices);

	//This is a 10 'unit' wide quad
	m_cubeTransform =
	{
		1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, 1, 0,
		0, 0, 0, 1
	};

	return true;
}

void GraphicsApp::CubeDraw(glm::mat4 pvm)
{
	// Bind the shader
	m_simpleShader.bind();

	// Bind the transform
	m_simpleShader.bindUniform("ProjectionViewModel", pvm);

	// Draw the quad using Mesh's draw
	m_cubeMesh.Draw();
}

bool GraphicsApp::BunnyLoader()
{
	m_phongShader.loadShader(aie::eShaderStage::VERTEX,
		"./shaders/phong.vert");
	m_phongShader.loadShader(aie::eShaderStage::FRAGMENT,
		"./shaders/phong.frag");

	if (!m_phongShader.link())
	{
		printf("Color Shader has an Error: %s\n",
			m_phongShader.getLastError());
		return false;
	}

	if (!m_bunnyMesh.load("./stanford/Bunny.obj"))
	{
		printf("Bunny Mesh has an Error!\n");
		return false;
	}

	m_bunnyTransform =
	{
		0.5f, 0, 0, 0,
		0, 0.5f, 0, 0,
		0, 0, 0.5f, 0,
		0, 0, 0, 1
	};
	m_bunnyTransform = glm::rotate(m_bunnyTransform, 90.f, glm::vec3(0, 1, 0));
	return true;
}

void GraphicsApp::BunnyDraw(glm::mat4 pvm)
{
	// Bind the shader
	m_phongShader.bind();

	// Bind the transform
	m_phongShader.bindUniform("ProjectionViewModel", pvm);

	// Draw the quad using Mesh's draw

	m_phongShader.bindUniform("BaseColor", glm::vec4(1, 1, 1, 1));

	m_bunnyMesh.draw();
}

bool GraphicsApp::SpearLoader()
{
	m_phongShader.loadShader(aie::eShaderStage::VERTEX,
		"./shaders/phong.vert");
	m_phongShader.loadShader(aie::eShaderStage::FRAGMENT,
		"./shaders/phong.frag");

	if (!m_phongShader.link())
	{
		printf("Color Shader has an Error: %s\n",
			m_phongShader.getLastError());
		return false;
	}

	if (!m_spearMesh.load("./soulspear/soulspear.obj"))
	{
		printf("SoulSpear Mesh has an Error!\n");
		return false;
	}

	m_spearTransform =
	{
		0, 1, 0, 0,
		0, 0, -1, 0,
		-1, 0, 0, 0,
		0, 0, 0, 1
	};
	m_spearTransform = glm::translate(m_spearTransform, glm::vec3(2.5f, -6.5f, -2.f));
	return true;
}

void GraphicsApp::SpearDraw(glm::mat4 pvm, glm::mat4 transform)
{
	// Bind the phong shader
	m_phongShader.bind();

	// Bind the directional light we defined
	m_phongShader.bindUniform("LightDirection", m_light.direction);
	m_phongShader.bindUniform("AmbientColor", m_ambientLight);
	m_phongShader.bindUniform("LightColor", m_light.color);

	// Bind the pvm using the one provided
	m_phongShader.bindUniform("ProjectionViewModel", pvm);

	// Bind the transform using the one provided
	m_phongShader.bindUniform("ModelMatrix", transform);
	m_spearMesh.draw();
}

bool GraphicsApp::CylinderLoader(float height, float radius, float segments)
{
	m_simpleShader.loadShader(aie::eShaderStage::VERTEX,
		"./shaders/simple.vert");
	m_simpleShader.loadShader(aie::eShaderStage::FRAGMENT,
		"./shaders/simple.frag");

	if (!m_simpleShader.link())
	{
		printf("Simple Shader has an Error: %s\n",
			m_simpleShader.getLastError());
		return false;
	}

	Mesh::Vertex* vertices = new Mesh::Vertex[(segments + 1) * 2];

	vertices[0].position = { 0, 0, 0, 0 };
	vertices[1].position = { 0, height, 0, 0 };
	vertices[2].position = { 1, height, 0, 0 };

	unsigned int* indices = new unsigned int[segments * 4 * 3];

	indices[0] = 0;
	indices[1] = 2;
	indices[2] = 1;

	m_cylinderMesh.Initialise(3, vertices, 3, indices);

	m_cylinderTransform =
	{
		10, 0, 0, 0,
		0, 10, 0, 0,
		0, 0, 10, 0,
		0, 0, 0, 1
	};

	return true;
}

void GraphicsApp::CylinderDraw(glm::mat4 pvm)
{
	// Bind the shader
	m_simpleShader.bind();

	// Bind the transform
	m_simpleShader.bindUniform("ProjectionViewModel", pvm);

	// Draw the quad using Mesh's draw
	m_cylinderMesh.Draw();
}

void GraphicsApp::PhongDraw(glm::mat4 pvm, glm::mat4 transform)
{
	// Bind the phong shader
	m_phongShader.bind();

	// Bind the camera position
	m_phongShader.bindUniform("CameraPosition",
		glm::vec3(glm::inverse(m_viewMatrix)[3]));

	// Bind the directional light we defined
	m_phongShader.bindUniform("LightDirection", m_light.direction);
	m_phongShader.bindUniform("AmbientColor", m_ambientLight);
	m_phongShader.bindUniform("LightColor", m_light.color);

	// Bind the pvm using the one provided
	m_phongShader.bindUniform("ProjectionViewModel", pvm);

	// Bind the transform using the one provided
	m_phongShader.bindUniform("ModelMatrix", transform);

	m_bunnyMesh.draw();
}

