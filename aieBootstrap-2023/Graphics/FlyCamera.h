#pragma once
#include "SimpleCamera.h"

class FlyCamera : public SimpleCamera
{
public:
	void Update(float deltaTime);
	void ImGui();

	void SetSpeed(float speed) { m_speed = speed; }
	float* GetSpeed() { return &m_speed; }
protected:
	float m_speed = 5;
	glm::vec3 m_up;
};

