#pragma once
#include "SimpleCamera.h"

class StationaryCamera : public SimpleCamera
{
public:
	StationaryCamera();

	void Update(float deltaTime);
	void ImGui();

	void SetRotation(glm::vec3 rotation);

};

