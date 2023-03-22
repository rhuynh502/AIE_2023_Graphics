#include "Instance.h"
#include <glm/ext.hpp>
#include "Scene.h"
#include "SimpleCamera.h"
#include "OBJMesh.h"
#include "Texture.h"
#include "GraphicsApp.h"

Instance::Instance(glm::mat4 transform, aie::OBJMesh* mesh,
	aie::ShaderProgram* shader) :
	m_transform(transform), m_mesh(mesh), m_shader(shader)
{

}

Instance::Instance(glm::vec3 position, glm::vec3 eulerAngles, glm::vec3 scale,
	aie::OBJMesh* mesh, aie::ShaderProgram* shader) :
	m_mesh(mesh), m_shader(shader)
{
	m_transform = MakeTransform(position, eulerAngles, scale);
}

void Instance::Draw(Scene* scene)
{
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
