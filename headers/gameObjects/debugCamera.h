#pragma once

#define NOMINMAX
#define WIN32_LEAN_AND_MEAN

#include "gameObjects/cameraObject.h"
#include "core/input/abstractInput.h"
#include "core/input/keyboardInput.h"

class DebugCamera : CameraObject {
public:
	DebugCamera() = default;

	KeyboardInput keyboardInput;

	virtual void Tick() override;
};