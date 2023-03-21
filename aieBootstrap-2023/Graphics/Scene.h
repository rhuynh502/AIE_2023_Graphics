#pragma once
#include <glm/glm.hpp>
#include <vector>
#include <list>

class SimpleCamera;
class Instance;

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

	SimpleCamera* GetCamera() { return m_camera; }
	//glm::vec2 GetWindowSize();
	glm::vec2 GetWindowSize() { return m_windowSize; }
	glm::vec3 GetAmbientLightColor() { return m_ambientLightColor; }
	Light GetLight() { return m_light; }
	std::list<Instance*> GetInstances() { return m_instances; }

protected:
	SimpleCamera* m_camera;
	glm::vec2 m_windowSize;
	Light m_light;
	glm::vec3 m_ambientLightColor;
	std::list<Instance*> m_instances;


};

