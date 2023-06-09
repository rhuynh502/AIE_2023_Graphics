#include "Instance.h"
#include <glm/ext.hpp>
#include "Scene.h"
#include "SimpleCamera.h"
#include "OBJMesh.h"
#include "Texture.h"
#include "GraphicsApp.h"
#include <imgui.h>

Instance::Instance(glm::mat4 transform, aie::OBJMesh* mesh,
	aie::ShaderProgram* shader, std::string instanceName) :
	m_transform(transform), m_mesh(mesh), m_shader(shader), m_instanceName(instanceName)
{
	m_baseTransform = m_transform;
}

Instance::Instance(glm::vec3 position, glm::vec3 eulerAngles, glm::vec3 scale,
	aie::OBJMesh* mesh, aie::ShaderProgram* shader, std::string instanceName) :
	m_mesh(mesh), m_shader(shader), m_instanceName(instanceName)
{
	m_transform = MakeTransform(position, eulerAngles, scale);
	m_baseTransform = m_transform;
}

void Instance::Draw(Scene* scene)
{
	if (!instanceOn)
		return;

	// Set the shader pipeline
	m_shader->bind();

	// Bind all relevant uniforms for our shaders
	auto pvm = scene->GetCamera()->GetProjectionMatrix(scene->GetWindowSize().x,
		scene->GetWindowSize().y) *
		scene->GetCamera()->GetViewMatrix() * m_transform;

	m_shader->bindUniform("ProjectionViewModel", pvm);
	m_shader->bindUniform("ModelMatrix", m_transform);

	// Bind the Camera Postion
	m_shader->bindUniform("CameraPosition",
		scene->GetCamera()->GetPosition());

	int numOfLights = scene->GetNumberOfLights();
	// Bind
	m_shader->bindUniform("numLights", numOfLights);
	m_shader->bindUniform("PointLightPositions", numOfLights, scene->GetPointLightPosition());
	m_shader->bindUniform("PointLightColors", numOfLights, scene->GetPointLightColor());

	// Bind the directional light we defined
	m_shader->bindUniform("LightDirection", scene->GetLight().direction);
	m_shader->bindUniform("LightColor", scene->GetLight().color);
	m_shader->bindUniform("AmbientColor", scene->GetAmbientLightColor());


	m_mesh->draw();
}

void Instance::ImGui()
{
	if (!instanceOn)
		return;

	if (ImGui::DragFloat3((m_instanceName + " Pos").c_str(), &m_transform[3][0], 0.05f))
		m_baseTransform[3] = m_transform[3];

	if (ImGui::DragFloat3((m_instanceName + " Rotation").c_str(), &m_curRotation[0], 0.1f))
	{
		glm::vec3 diff = m_curRotation - m_prevRotation;
		m_transform = glm::rotate(m_transform, diff[0], glm::vec3(1, 0, 0));
		m_transform = glm::rotate(m_transform, diff[1], glm::vec3(0, 1, 0));
		m_transform = glm::rotate(m_transform, diff[2], glm::vec3(0, 0, 1));
		m_prevRotation = m_curRotation;
	}

	if (ImGui::DragFloat((m_instanceName + " Scale").c_str(), &m_scale, 0.05f, 0.1f, INFINITY))
	{
		float diff = m_scale / m_prevScale;
		m_transform = glm::scale(glm::translate(m_transform, {0, 0, 0}), glm::vec3(diff));
		m_prevScale = m_scale;
	}


}

glm::mat4 Instance::MakeTransform(glm::vec3 position, glm::vec3 eulerAngles, glm::vec3 scale)
{
	return glm::translate(glm::mat4(1), position) *
		glm::rotate(glm::mat4(1),
			glm::radians(eulerAngles.z), glm::vec3(0, 0, 1)) *
		glm::rotate(glm::mat4(1),
			glm::radians(eulerAngles.y), glm::vec3(0, 1, 0)) *
		glm::rotate(glm::mat4(1),
			glm::radians(eulerAngles.x), glm::vec3(1, 0, 0)) *
		glm::scale(glm::mat4(1), scale);
			
}
