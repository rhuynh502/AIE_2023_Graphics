#pragma once
#include "SimpleCamera.h"

class StationaryCamera : public SimpleCamera
{
public:

	void Update(float deltaTime);

	void SetRotation(glm::vec3 rotation);

};

