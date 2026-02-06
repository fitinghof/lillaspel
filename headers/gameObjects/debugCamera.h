#pragma once

#define NOMINMAX
#define WIN32_LEAN_AND_MEAN

#include "gameObjects/cameraObject.h"

class DebugCamera : CameraObject {
public:
	DebugCamera() = default;

	virtual void Tick() override;
};