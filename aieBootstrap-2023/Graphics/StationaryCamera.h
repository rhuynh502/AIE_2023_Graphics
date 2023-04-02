#pragma once
#include "SimpleCamera.h"

class StationaryCamera : public SimpleCamera
{
public:
	StationaryCamera();

	void ImGui() override;

	void SetRotation(glm::vec3 rotation);

};

