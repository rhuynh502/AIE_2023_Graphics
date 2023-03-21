#pragma once

#include "Application.h"
#include <glm/mat4x4.hpp>
#include "Planet.h"
#include "Mesh.h"
#include "Shader.h"
#include "OBJMesh.h"
#include "SimpleCamera.h"
#include "StationaryCamera.h"
#include "Scene.h"
#include "Instance.h"

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
	void ImGUIRefresher();

	bool QuadLoader();
	void QuadDraw(glm::mat4 pvm);

	bool CubeLoader();
	void CubeDraw(glm::mat4 pvm);

	bool PyramidLoader();
	void PyramidDraw(glm::mat4 pvm);

	bool BunnyLoader();
	void BunnyDraw(glm::mat4 pvm);

	bool SpearLoader();
	void OBJDraw(glm::mat4 pvm, glm::mat4 transform, aie::OBJMesh* objMesh);

	bool RobotLoader();
	bool SaberLoader();

	bool CylinderLoader(float height, float radius, float segments);
	void CylinderDraw(glm::mat4 pvm);

	bool QuadTexturedLoader();
	void QuadTexturedDraw(glm::mat4 pvm);

	void PhongDraw(glm::mat4 pvm, glm::mat4 transform);

	Scene* m_scene;

	// camera transforms
	glm::mat4	m_viewMatrix;
	glm::mat4	m_projectionMatrix;

	aie::Texture m_gridTexture;

	aie::ShaderProgram m_simpleShader;
	aie::ShaderProgram m_colorShader;
	aie::ShaderProgram m_phongShader;
	aie::ShaderProgram m_texturedShader;
	aie::ShaderProgram m_normallitShader;

	Mesh m_quadMesh;
	glm::mat4 m_quadTransform;

	Mesh m_cubeMesh;
	glm::mat4 m_cubeTransform;

	Mesh m_cylinderMesh;
	glm::mat4 m_cylinderTransform;

	Mesh m_pyramidMesh;
	glm::mat4 m_pyramidTransform;

	aie::OBJMesh m_bunnyMesh;
	glm::mat4 m_bunnyTransform;

	aie::OBJMesh m_spearMesh;
	glm::mat4 m_spearTransform;

	aie::OBJMesh m_robotMesh;
	glm::mat4 m_robotTransform;

	aie::OBJMesh m_saberMesh;
	glm::mat4 m_saberTransform;

	SimpleCamera* m_mainCamera;
	SimpleCamera m_camera;
	StationaryCamera m_stationaryCamera;

	//struct Light {
	//	glm::vec3 direction;
	//	glm::vec3 color;
	//};
	Light m_light;
	glm::vec3 m_ambientLight;

	std::vector<Planet*> m_planets;
	Planet* m_sun;
	bool m_planetsOn = false;
	bool m_cubeOn = false;
	bool m_cylinderOn = false;
	bool m_pyramidOn = false;
	bool m_bunnyOn = false;
	bool m_quadOn = false;
};