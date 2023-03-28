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
	m_mainCamera = &m_camera;

	setBackgroundColour(0.25f, 0.25f, 0.25f);

	// initialise gizmo primitive counts
	Gizmos::create(10000, 10000, 10000, 10000);

	// create simple camera transforms
	m_viewMatrix = glm::lookAt(vec3(10), vec3(0), vec3(0, 1, 0));
	m_projectionMatrix = glm::perspective(glm::pi<float>() * 0.25f, 16.0f / 9.0f, 0.1f, 1000.0f);

	m_ambientLight = { 0.5, 0.5, 0.5 };

	InitialisePlanets();

	m_stationaryCamera.SetPosition(glm::vec3(10, 3, 0));
	m_stationaryCamera.SetRotation(glm::vec3(180, 0, 0));
	m_stationaryCamera1.SetPosition(glm::vec3(-10, 3, 0));
	m_stationaryCamera1.SetRotation(glm::vec3(0, 0, 0));

	Light light;
	light.color = { 1, 1, 1 };
	light.direction = { 1, -1, 1 };

	m_emitter = new ParticleEmitter();
	m_emitter->Initialise(1000, 500, .1f, 1.0f, 1, 5, 1, .1f, 
		glm::vec4(0, 0, 1, 1), glm::vec4(0.3f, 0, 1, 1));

	m_scene = new Scene(m_mainCamera, glm::vec2(getWindowWidth(), getWindowHeight()),
		light, m_ambientLight);
	m_mainCamera->isMainCamera = true;

	m_scene->AddPointLights(glm::vec3(-3, 2, 0), glm::vec3(1, 0, 0), 30);
	m_scene->AddPointLights(glm::vec3(3, 2, 0), glm::vec3(0, 0, 1), 30);

	m_cameras.push_back(m_camera);
	m_cameras.push_back(m_stationaryCamera);
	m_cameras.push_back(m_stationaryCamera1);

	return LaunchShaders();
}

void GraphicsApp::shutdown() {

	Gizmos::destroy();
	delete m_scene;
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

	m_sun->Update(deltaTime);

	// quit if we press escape
	aie::Input* input = aie::Input::getInstance();

	m_mainCamera->Update(deltaTime);

	// Rotate the light to emulate a 'day/night' cycle
	m_scene->GetLight().direction = glm::normalize(glm::vec3(glm::cos(time * 2), glm::sin(time * 2), 0));

	m_emitter->Update(deltaTime, m_mainCamera->GetWorldTransform(
		m_mainCamera->GetPosition(), glm::vec3(0), glm::vec3(1)));

	if (input->isKeyDown(aie::INPUT_KEY_ESCAPE))
		quit();

	ImGUIRefresher();
}

void GraphicsApp::draw()
{
	// Bind the render target as the
	// first part of the draw function
	m_renderTarget.bind();

	// wipe the screen to the background colour
	clearScreen();

	for (auto camera : m_cameras)
		camera.Draw();
	// update perspective based on screen size
	// create simple camera transforms
	m_viewMatrix = m_mainCamera->GetViewMatrix();
	m_projectionMatrix = m_mainCamera->GetProjectionMatrix((float)getWindowWidth(),
		(float)getWindowHeight());

	auto pv = m_projectionMatrix * m_viewMatrix;

	if(m_cubeOn)
		CubeDraw(pv * m_cubeTransform);

	if(m_quadOn)
		QuadTexturedDraw(pv * m_quadTransform);

	if (m_bunnyOn)
	{
		PhongDraw(pv * m_bunnyTransform, m_bunnyTransform);
		OBJDraw(pv, m_spearTransform, &m_spearMesh);
	}

	if(m_cylinderOn)
		CylinderDraw(pv * m_cylinderTransform);

	if(m_pyramidOn)
		PyramidDraw(pv * m_pyramidTransform);

	m_sun->Draw();

	m_scene->Draw();

	m_particleShader.bind();
	m_particleShader.bindUniform("ProjectionViewModel", pv * m_particleEmitTransform);
	m_emitter->Draw();

	Gizmos::draw(m_projectionMatrix * m_viewMatrix);

	// Unbind the target to return to the backbuffer
	m_renderTarget.unbind();

	clearScreen();


	//OBJDraw(pv, m_robotTransform, &m_robotMesh);
	// Bind the post processing shader and the texture
	m_postProcessShader.bind();
	m_postProcessShader.bindUniform("colorTarget", 0);
	m_postProcessShader.bindUniform("postProcessTarget", m_postProcessEffect);
	m_postProcessShader.bindUniform("windowWidth", (int)getWindowWidth());
	m_postProcessShader.bindUniform("windowHeight", (int)getWindowHeight());
	m_postProcessShader.bindUniform("posterNumColors", m_posterNumColors);
	m_postProcessShader.bindUniform("posterGamma", m_posterGamma);
	m_postProcessShader.bindUniform("pixels", m_pixels);
	m_postProcessShader.bindUniform("pixelWidth", m_pixelWidth);
	m_postProcessShader.bindUniform("pixelHeight", m_pixelHeight);
	m_renderTarget.getTarget(0).bind(0);

	m_fullScreenQuad.Draw();

}

void GraphicsApp::InitialisePlanets()
{
	// initialise planets
	m_sun = new Planet(mat4(1), 0, vec4(1, 0, 0, 1), 0, 0.1f, "Sun");
	m_sun->SetMatrix(glm::translate(m_sun->GetMatrix(), vec3(0, 1, 0)));

	Planet* mercury = new Planet(glm::translate(m_sun->GetMatrix(), vec3(1.2f, 0, 0)), 1.2f, vec4(0.5f, 0.5f, 0, 1), 1.2f, 0.2f, "Mercury");
	m_sun->AddChild(mercury);
	mercury->SetAxis(-1);

	Planet* venus = new Planet(glm::translate(m_sun->GetMatrix(), vec3(1.6f, 0, 0)), 1.6f, vec4(1, 0.647f, 0, 1), 0.8f, 0.2f, "Venus");
	m_sun->AddChild(venus);

	Planet* earth = new Planet(glm::translate(m_sun->GetMatrix(), vec3(2.f, 0, 0)), 2.f, vec4(0.2f, 0.2f, 1, 1), 0.6f, 0.2f, "Earth");
	earth->SetParentPlanet(m_sun);

	Planet* mars = new Planet(glm::translate(m_sun->GetMatrix(), vec3(2.4f, 0, 0)), 2.4f, vec4(1, 0.7f, 0, 1), 0.7f, 0.2f, "Mars");
	mars->SetParentPlanet(m_sun);
	mars->SetAxis(-0.64f);

	Planet* jupiter = new Planet(glm::translate(m_sun->GetMatrix(), vec3(2.8f, 0, 0)), 2.8f, vec4(1, 0.87f, 0, 1), 0.45f, 0.2f, "Jupiter", true);
	jupiter->SetParentPlanet(m_sun);
	jupiter->SetAxis(0.77f);

	Planet* saturn = new Planet(glm::translate(m_sun->GetMatrix(), vec3(3.2f, 0, 0)), 3.2f, vec4(1, 1, 1, 1), 0.4f, 0.2f, "Saturn", true);
	saturn->SetParentPlanet(m_sun);
	saturn->SetAxis(-0.2f);

	Planet* uranus = new Planet(glm::translate(m_sun->GetMatrix(), vec3(3.6f, 0, 0)), 3.6f, vec4(0, 0, 1, 1), 0.2f, 0.2f, "Uranus");
	uranus->SetParentPlanet(m_sun);
	uranus->SetAxis(0.466f);

	Planet* moon = new Planet(glm::translate(earth->GetMatrix(), vec3(0.2f, 0, 0)), 0.2f, vec4(1, 1, 1, 1), 1.5f, 0.05f, "Earth Moon");
	earth->AddChild(moon);
	moon->SetAxis(0.6f);

	m_particleEmitTransform = earth->GetMatrix();
}

bool GraphicsApp::LaunchShaders()
{
	if (!m_renderTarget.initialise(1, getWindowWidth(), getWindowHeight()))
	{
		printf("Render target error!\n");
		return false;
	}

#pragma region LoadingShaders
	m_normallitShader.loadShader(aie::eShaderStage::VERTEX,
		"./shaders/normallit.vert");
	m_normallitShader.loadShader(aie::eShaderStage::FRAGMENT,
		"./shaders/normallit.frag");

	if (!m_normallitShader.link())
	{
		printf("Shader Error %s\n", m_normallitShader.getLastError());
		return false;
	}

	// Post Processing Shader
	m_postProcessShader.loadShader(aie::eShaderStage::VERTEX,
		"./shaders/post.vert");
	m_postProcessShader.loadShader(aie::eShaderStage::FRAGMENT,
		"./shaders/post.frag");

	if (!m_postProcessShader.link())
	{
		printf("Post Processing Shader has an Error: %s\n",
			m_postProcessShader.getLastError());
		return false;
	}

	// Particle Shader
	m_particleShader.loadShader(aie::eShaderStage::VERTEX,
		"./shaders/particle.vert");
	m_particleShader.loadShader(aie::eShaderStage::FRAGMENT,
		"./shaders/particle.frag");

	if (!m_particleShader.link())
	{
		printf("Particle Shader has an Error: %s\n",
			m_particleShader.getLastError());
		return false;
	}
#pragma endregion 

#pragma region LoadingMeshes
	// Used to load cube
	if (!CubeLoader())
		return false;

	// Used to load OBJ bunny
	if (!BunnyLoader())
		return false;

	// Used to load OBJ spear
	if (!SpearLoader())
		return false;

	if (!CylinderLoader(2, 1, 12))
		return false;

	if (!PyramidLoader())
		return false;

	if (!QuadTexturedLoader())
		return false;

	if (!RobotLoader())
		return false;

	// Create a full screen quad
	m_fullScreenQuad.InitialiseFullScreenQuad();
#pragma endregion

#pragma region LoadingInstances
	for(int i = 0; i < 6; i++)
		m_scene->AddInstance(new Instance(glm::vec3(-42 + (i * 14), 0, 0), glm::vec3(0, i * 30, 0),
			glm::vec3(1, 1, 1), &m_robotMesh, &m_normallitShader, "Robot " + std::to_string(i)));
#pragma endregion
	return true;
}

void GraphicsApp::ImGUIRefresher()
{
	ImGui::Begin("Settings");
	ImGui::DragFloat3("Global Light Color", 
		&m_scene->GetLight().color[0], 0.05, 0, 1);
	ImGui::DragFloat3("Global Light Direction",
		&m_scene->GetLight().direction[0], 0.05, -1, 1);
	if (ImGui::CollapsingHeader("Camera Settings"))
	{
		if (ImGui::Button("StationaryCamera"))
		{
			m_mainCamera = &m_stationaryCamera;
			m_scene->SetCamera(m_mainCamera);
			m_mainCamera->ToggleCamera();
		}
		if (ImGui::Button("StationaryCamera1"))
		{
			m_mainCamera = &m_stationaryCamera1;
			m_scene->SetCamera(m_mainCamera);
			m_mainCamera->ToggleCamera();
		}
		if (ImGui::Button("FlyCamera"))
		{
			m_mainCamera = &m_camera;
			m_scene->SetCamera(m_mainCamera);
			m_mainCamera->ToggleCamera();
		}
		m_mainCamera->ImGui();
	}

	if (ImGui::CollapsingHeader("Post Processing"))
	{
		ImGui::SliderInt("Post Processing Effect", &m_postProcessEffect, -1, 11);
		if (m_postProcessEffect == 7)
		{
			ImGui::SliderInt("Amount of Pixels", &m_pixels, 32, 2056);
			ImGui::SliderFloat("Pixel Width", &m_pixelWidth, 0.1f, 25);
			ImGui::SliderFloat("Pixel Height", &m_pixelHeight, 0.1f, 25);
		}
		if (m_postProcessEffect == 8)
		{
			ImGui::SliderFloat("Number of Colors", &m_posterNumColors, 3.2f, 20);
			ImGui::SliderFloat("Gamma", &m_posterGamma, 0.1, 1.5f);
		}
	}

	if (ImGui::CollapsingHeader("Planets Settings"))
	{
		ImGui::Checkbox(m_sun->GetName(), &m_sun->planetOn);
		
		if(ImGui::Button(m_bunnyOn ? "DEACTIVATE BUNNY" : "ACTIVATE BUNNY"))
			m_bunnyOn = !m_bunnyOn;
	}

	if (ImGui::CollapsingHeader("Shapes Settings"))
	{
		ImGui::Checkbox(!m_cubeOn ? "Cube off" : "Cube On", &m_cubeOn);
		if(m_cubeOn)
			ImGui::DragFloat3("Cube Position",
				&m_cubeTransform[3][0]);
		ImGui::Checkbox(!m_cylinderOn ? "Cylinder off" : "Cylinder On", &m_cylinderOn);
		if(m_cylinderOn)
			ImGui::DragFloat3("Cylinder Position",
				&m_cylinderTransform[3][0]);
		ImGui::Checkbox(!m_quadOn ? "Quad off" : "Quad On", &m_quadOn);
		if(m_quadOn)
			ImGui::DragFloat3("Quad Position",
				&m_quadTransform[3][0]);
		ImGui::Checkbox(!m_pyramidOn ? "Pyramid off" : "Pyramid On", &m_pyramidOn);
		if(m_pyramidOn)
			ImGui::DragFloat3("Pyramid Position",
				&m_pyramidTransform[3][0]);
	}
	ImGui::End();

	m_scene->ImGui();
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

	glm::vec4 center = { 0, 1, 0, 0 };
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

bool GraphicsApp::PyramidLoader()
{
	// Defined as 4 vertices for the 2 triangles
	Mesh::Vertex vertices[5];

	glm::vec4 center = { 0, 1, 0, 0 };
	float distFromCenter = 1;

	vertices[0].position = { center.x + distFromCenter, center.y - distFromCenter, center.z + distFromCenter, 1 };
	vertices[1].position = { center.x + distFromCenter, center.y - distFromCenter, center.z - distFromCenter, 1 };
	vertices[2].position = { center.x - distFromCenter, center.y - distFromCenter, center.z + distFromCenter, 1 };
	vertices[3].position = { center.x - distFromCenter, center.y - distFromCenter, center.z - distFromCenter, 1 };
	vertices[4].position = { 0, distFromCenter, 0, 1 };
	
	unsigned int indices[18] = { 4, 0, 1, 4, 1, 3,
								 4, 3, 2, 4, 2, 0,
								 1, 0, 3, 3, 0, 2};

	m_pyramidMesh.Initialise(5, vertices, 18, indices);

	//This is a 10 'unit' wide quad
	m_pyramidTransform =
	{
		1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, 1, 0,
		0, 0, 0, 1
	};

	return true;
}

void GraphicsApp::PyramidDraw(glm::mat4 pvm)
{
	// Bind the shader
	m_simpleShader.bind();

	// Bind the transform
	m_simpleShader.bindUniform("ProjectionViewModel", pvm);

	// Draw the quad using Mesh's draw
	m_pyramidMesh.Draw();
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
	if (!m_spearMesh.load("./soulspear/soulspear.obj", true, true))
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

void GraphicsApp::OBJDraw(glm::mat4 pvm, glm::mat4 transform, aie::OBJMesh* objMesh)
{
	// Bind the phong shader
	m_normallitShader.bind();

	m_normallitShader.bindUniform("CameraPosition",
		glm::vec3(glm::inverse(m_viewMatrix)[3]));

	// Bind the directional light we defined
	m_normallitShader.bindUniform("LightDirection", m_scene->GetLight().direction);
	m_normallitShader.bindUniform("AmbientColor", m_scene->GetAmbientLightColor());
	m_normallitShader.bindUniform("LightColor", m_scene->GetLight().color);

	//Bind Texture location
	m_normallitShader.bindUniform("diffuseTexture", 0);

	// Bind the pvm using the one provided
	m_normallitShader.bindUniform("ProjectionViewModel", pvm * transform);

	// Bind the transform using the one provided
	m_normallitShader.bindUniform("ModelMatrix", transform);
	objMesh->draw();
}

bool GraphicsApp::OBJLoader(aie::OBJMesh& objMesh, glm::mat4& transform, float scale, const char* filepath, bool flipTexture)
{
	return false;
}

bool GraphicsApp::RobotLoader()
{
	if (!m_robotMesh.load("./robo-obj-pose4/robo-obj-pose4.obj", true, true
	))
	{
		printf("Robot Mesh has an Error!\n");
		return false;
	}

	m_robotTransform =
	{
		0.3f, 0, 0, 0,
		0, 0.3f, 0, 0,
		0, 0, 0.3f, 0,
		0, 0, 0, 1
	};
	
	return true;
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

	float verticesCount = (segments + 1) * 2;

	Mesh::Vertex* vertices = new Mesh::Vertex[verticesCount];

	// Create center vertices for top and bottom
	vertices[0].position = { 0, 0, 0, 1 };
	vertices[1].position = { 0, height, 0, 1 };

	// Create vertices for top circle and bottom circle
	for (int i = 0; i < segments * 2; i++)
	{
		vertices[i + 2].position = {radius * glm::sin((i % (int)segments) * 2 * glm::pi<float>() / segments), 
									i < segments ? 0 : height, 
									radius * glm::cos((i % (int)segments) * 2 * glm::pi<float>() / segments), 1};
	}

	unsigned int* indices = new unsigned int[segments * 4 * 3];

	// Create triangles for top circle
	for (int i = 0; i < segments; i++)
	{
		indices[i * 3] = 0;
		indices[i * 3 + 1] = i == segments - 1 ? 2 : 2 + i + 1;
		indices[i * 3 + 2] = 2 + i;
	}

	// Create triangles for bot circle
	for (int i = segments; i < segments * 2; i++)
	{
		indices[i * 3] = 1;
		indices[i * 3 + 1] = 2 + i;
		indices[i * 3 + 2] = i == segments * 2 - 1 ? 2 + segments : 2 + i + 1;
	}

	int startingPoint = 2;
	int midPoint = verticesCount / 2 + 1;
	int increment = 0;

	// Create triangles for faces
	for (int i = segments * 2; i < segments * 3; i++)
	{
		indices[i * 3] = startingPoint + increment;
		indices[i * 3 + 1] = i == segments * 3 - 1 ? startingPoint : startingPoint + 1 + increment;
		indices[i * 3 + 2] = i == segments * 3 - 1 ? midPoint : startingPoint + midPoint + increment - 1;
		
		increment++;
	}
	
	increment = 0;

	// Create triangles for faces
	for (int i = segments * 3; i < segments * 4; i++)
	{
		indices[i * 3] = midPoint + increment;
		indices[i * 3 + 1] = startingPoint + increment;
		indices[i * 3 + 2] = i == segments * 4 - 1 ? midPoint: midPoint + 1 + increment;
		
		increment++;
	}

	m_cylinderMesh.Initialise(verticesCount, vertices, segments * 4 * 3, indices);

	m_cylinderTransform =
	{
		1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, 1, 0,
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

bool GraphicsApp::QuadTexturedLoader()
{
	m_texturedShader.loadShader(aie::eShaderStage::VERTEX,
		"./shaders/textured.vert");
	m_texturedShader.loadShader(aie::eShaderStage::FRAGMENT,
		"./shaders/textured.frag");

	if (!m_texturedShader.link())
	{
		printf("Textured Shader has an Error: %s\n",
			m_texturedShader.getLastError());
		return false;
	}

	if (!m_gridTexture.load("./textures/numbered_grid.tga"))
	{
		printf("Failed to load the grid texture correctly!\n");
		return false;
	}

	m_quadMesh.InitialiseQuad();

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

void GraphicsApp::QuadTexturedDraw(glm::mat4 pvm)
{
	// Bind the shader
	m_texturedShader.bind();

	// Bind the transform
	m_texturedShader.bindUniform("ProjectionViewModel", pvm);

	// Bind the texture location
	m_texturedShader.bindUniform("diffuseTexture", 0);

	// Bind the texture to a specific location
	//m_gridTexture.bind(0);

	m_renderTarget.getTarget(0).bind(0);

	// Draw the quad using Mesh's draw
	m_quadMesh.Draw();
}

void GraphicsApp::PhongDraw(glm::mat4 pvm, glm::mat4 transform)
{
	// Bind the phong shader
	m_phongShader.bind();

	// Bind the camera position
	m_phongShader.bindUniform("CameraPosition",
		glm::vec3(glm::inverse(m_viewMatrix)[3]));

	// Bind the directional light we defined
	m_phongShader.bindUniform("LightDirection", m_scene->GetLight().direction);
	m_phongShader.bindUniform("AmbientColor", m_scene->GetAmbientLightColor());
	m_phongShader.bindUniform("LightColor", m_scene->GetLight().color);

	// Bind the pvm using the one provided
	m_phongShader.bindUniform("ProjectionViewModel", pvm);

	// Bind the transform using the one provided
	m_phongShader.bindUniform("ModelMatrix", transform);

	m_bunnyMesh.draw();
}

