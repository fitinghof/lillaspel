#pragma once

#define NOMINMAX
#define WIN32_LEAN_AND_MEAN

#include "gameObjects/cameraObject.h"
#include "core/input/abstractInput.h"
#include "core/input/keyboardInput.h"
#include "core/input/controllerInput.h"

class DebugCamera : CameraObject {
public:
	DebugCamera() : controllerInput(DWORD(0)) {}

	KeyboardInput keyboardInput;
	ControllerInput controllerInput;

	virtual void Tick() override;
};