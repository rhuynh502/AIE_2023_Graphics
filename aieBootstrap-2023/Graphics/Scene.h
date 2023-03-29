#pragma once
#include <glm/glm.hpp>
#include <vector>
#include <list>
#include "SimpleCamera.h"
#include "Gizmos.h"

class Instance;

const int MAX_LIGHTS = 4;

struct Light 
{
	Light() 
	{
		direction = glm::vec3(0);
		color = glm::vec3(1);
	};
	Light(glm::vec3 position, glm::vec3 colour,
		float intensity)
	{
		direction = position;
		color = colour * intensity;
	}
	~Light() {};

	void Draw()
	{
		aie::Gizmos::addSphere(direction, 0.5f, 7, 7, glm::vec4(color, 0));
	}

	glm::vec3 direction;
	glm::vec3 color;
};

class Scene
{
public:
	Scene(SimpleCamera* camera, glm::vec2 windowSize,
		Light& light, glm::vec3 ambientLightColor);
	~Scene();

	void AddInstance(Instance* instance);

	void Draw();
	void ImGui();
	void AddPointLights(Light light) { m_pointLights.push_back(light); }
	void AddPointLights(glm::vec3 direction, glm::vec3 color, float intensity)
	{
		m_pointLights.push_back(Light(direction, color, intensity));
	}


	SimpleCamera* GetCamera() { return m_camera; }
	//glm::vec2 GetWindowSize();
	glm::vec2 GetWindowSize() { return m_windowSize; }
	glm::vec3 GetAmbientLightColor() { return m_ambientLightColor; }
	Light& GetLight() { return m_light; }
	std::list<Instance*> GetInstances() { return m_instances; }
	std::vector<Light>& GetPointLights() { return m_pointLights; }
	int GetNumberOfLights() { return m_pointLights.size(); }
	glm::vec3* GetPointLightPosition() { return &m_pointLightPositions[0]; }
	glm::vec3* GetPointLightColor() { return &m_pointLightColors[0]; }

	void SetCamera(SimpleCamera* camera) { m_camera = camera; }

protected:
	SimpleCamera* m_camera;
	glm::vec2 m_windowSize;

	Light m_sunLight;
	std::vector<Light> m_pointLights;

	Light m_light;
	glm::vec3 m_ambientLightColor;
	std::list<Instance*> m_instances;

	glm::vec3 m_pointLightPositions[MAX_LIGHTS];
	glm::vec3 m_pointLightColors[MAX_LIGHTS];
};

