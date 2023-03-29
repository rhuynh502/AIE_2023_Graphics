#include "Scene.h"
#include "Instance.h"
#include "imgui.h"

Scene::Scene(SimpleCamera* camera, glm::vec2 windowSize, 
	Light& light, glm::vec3 ambientLightColor) : 
	m_camera(camera), m_windowSize(windowSize), 
	m_light(light), m_ambientLightColor(ambientLightColor)
{
	
}

Scene::~Scene()
{
	for (auto it = m_instances.begin(); it != m_instances.end(); it++)
	{
		delete* it;
	}
}

void Scene::AddInstance(Instance* instance)
{
	m_instances.push_back(instance);
}

void Scene::Draw()
{
	for (int i = 0; i < m_pointLights.size() && i < MAX_LIGHTS; i++)
	{
		m_pointLightPositions[i] = m_pointLights[i].direction;
		m_pointLightColors[i] = m_pointLights[i].color;
		m_pointLights[i].Draw();
	}
	m_light.Draw();
	for (auto it = m_instances.begin(); it != m_instances.end(); it++)
	{
		Instance* instance = *it;
		instance->Draw(this);
	}
}

void Scene::ImGui()
{
	ImGui::Begin("Instances");
	
	for (auto instance : m_instances)
	{
		ImGui::Checkbox(instance->GetName().c_str(), &instance->instanceOn);
	}
	

	ImGui::End();
}

