#pragma once
#include <glm/glm.hpp>
#include <string>

class SimpleCamera;
class Scene;
struct Light;

namespace aie {
	class OBJMesh;
	class ShaderProgram;
}

class Instance
{
public:
	Instance(glm::mat4 transform, aie::OBJMesh* mesh,
		aie::ShaderProgram* shader, std::string instanceName);
	Instance(glm::vec3 position, glm::vec3 eulerAngles, glm::vec3 scale,
		aie::OBJMesh* mesh, aie::ShaderProgram* shader, std::string instanceName);
	~Instance() {};

	void Draw(Scene* scene);
	void ImGui();

	void ToggleOn() { instanceOn = !instanceOn; }

	static glm::mat4 MakeTransform(glm::vec3 position,
		glm::vec3 eulerAngles, glm::vec3 scale);

	std::string GetName() { return m_instanceName; }
	bool GetOnOff() { return instanceOn; }

	bool instanceOn = false;
protected:
	glm::mat4 m_transform;
	glm::mat4 m_baseTransform;
	aie::OBJMesh* m_mesh;
	aie::ShaderProgram* m_shader;
	std::string m_instanceName;
	float m_rotationX = 0;
	float m_rotationY = 0;
	float m_rotationZ = 0;
	glm::vec3 m_curRotation = glm::vec3(0);
	glm::vec3 m_prevRotation = glm::vec3(0);

	float m_scale = 1;
	float m_prevScale = 1;
};

