#include "FlyCamera.h"
#include "Input.h"
#include "imgui.h"

FlyCamera::FlyCamera()
{
	m_position = { 10, 3, 0 };
	m_phi = 0;
	m_theta = 180;
}

void FlyCamera::Update(float deltaTime)
{
	aie::Input* input = aie::Input::getInstance();
	float thetaR = glm::radians(m_theta);
	float phiR = glm::radians(m_phi);

	glm::vec3 forward(glm::cos(phiR) * glm::cos(thetaR), glm::sin(phiR),
		glm::cos(phiR) * glm::sin(thetaR));
	glm::vec3 right(-glm::sin(thetaR), 0, glm::cos(thetaR));
	glm::vec3 up(0.f, 1.f, 0.f);

	// WASD to move and QE to up and down
	if (input->isKeyDown(aie::INPUT_KEY_W))
		m_position += forward * deltaTime * m_speed;
	if (input->isKeyDown(aie::INPUT_KEY_S))
		m_position -= forward * deltaTime * m_speed;
	if (input->isKeyDown(aie::INPUT_KEY_D))
		m_position += right * deltaTime * m_speed;
	if (input->isKeyDown(aie::INPUT_KEY_A))
		m_position -= right * deltaTime * m_speed;

	if (input->isKeyDown(aie::INPUT_KEY_Q))
		m_position += up * deltaTime * m_speed;
	if (input->isKeyDown(aie::INPUT_KEY_E))
		m_position -= up * deltaTime * m_speed;

	// Get Mouse Coordinates
	float mx = input->getMouseX();
	float my = input->getMouseY();

	// If the right mouse button is held down, rotate camera
	if (input->isMouseButtonDown(1))
	{
		m_theta += m_rotationSpeed * (mx - m_lastMouse.x) * deltaTime;
		m_phi += m_rotationSpeed * (my - m_lastMouse.y) * deltaTime;
	}

	m_lastMouse = glm::vec2(mx, my);
}


void FlyCamera::ImGui()
{
	ImGui::DragFloat("Rotation Speed", &m_rotationSpeed,
		0.1f, 0.1f, 40.f);

	ImGui::DragFloat("Movement Speed", &m_speed,
		0.1f, 0.1f, 5.f);

}

