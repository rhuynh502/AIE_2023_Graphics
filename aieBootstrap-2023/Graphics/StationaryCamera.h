#pragma once
#include "SimpleCamera.h"

class StationaryCamera : public SimpleCamera
{
public:
	StationaryCamera();
	void Draw() override;
	void ImGui() override;

	void SetRotation(glm::vec3 rotation);

};

