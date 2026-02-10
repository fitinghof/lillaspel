#include "gameObjects/debugCamera.h"

#include "core/input/inputManager.h"
#include "utilities/time.h"
#include "utilities/logger.h"

#include "imgui.h"
void DebugCamera::Tick()
{
	this->CameraObject::Tick();

	InputManager::GetInstance().ReadControllerInput(DWORD(0));

	//Logger::Log(std::to_string(InputManager::GetInstance().GetConnectedControllerCount()));

	//this->controllerInput.PrintIndex();

	std::array<float, 2> movementVector = this->controllerInput.GetMovementVector();
	if (movementVector[0] != 0.0f || movementVector[1] != 0.0f) {
		Logger::Log("Controller movement vector: " + std::to_string(movementVector[0]) + ", " + std::to_string(movementVector[1]));
	}

	if (this->controllerInput.Jump()) {
		Logger::Log("Jump pressed");
	}

	if(this->controllerInput.LeftClick()) {
		Logger::Log("Left click pressed");
	}

	if(this->controllerInput.Quit()) {
		PostQuitMessage(0);
	}
	
	// ====== HAMPUS TEST SKIT :) ======
	/*if (keyboardInput.Quit()) {
		PostQuitMessage(0);
	}*/

	/*if (keyboardInput.Jump()) {
		Logger::Log("Jump pressed");
	}
	
	std::array movementVector = keyboardInput.GetMovementVector();
	if (movementVector[0] != 0.0f || movementVector[1] != 0.0f) {
		Logger::Log("Movement vector: " + std::to_string(movementVector[0]) + ", " + std::to_string(movementVector[1]));
	}*/

	//std::array<float, 2> lookVector = keyboardInput.GetLookVector();
	//
	//POINT screenCenter{
	//		.x = GetSystemMetrics(SM_CXSCREEN) / 2,
	//		.y = GetSystemMetrics(SM_CYSCREEN) / 2,
	//};

	//// Get current cursor
	//POINT cursorPos;
	//GetCursorPos(&cursorPos);

	//// Calculate mouse movement
	//int mouseDeltaX = cursorPos.x - screenCenter.x;
	//int mouseDeltaY = cursorPos.y - screenCenter.y;

	//// Reset cursor pos to center
	//SetCursorPos(screenCenter.x, screenCenter.y);

	//// Mouse movement
	//const float sensitivity = 0.001f;
	//const float clampMargin = 0.3f;
	//float x = mouseDeltaX * sensitivity;
	//float y = mouseDeltaY * sensitivity;

	//DirectX::XMVECTOR qRot = this->transform.GetRotationQuaternion();
	//float xRotation = DirectX::XMVectorGetX(qRot);
	//float yRotation = DirectX::XMVectorGetY(qRot);

	//xRotation += y;
	//xRotation = std::clamp(xRotation, (-DirectX::XM_PIDIV2) + clampMargin, DirectX::XM_PIDIV2 - clampMargin);
	//yRotation += x;

	//this->transform.SetRotationQuaternion(DirectX::XMQuaternionRotationRollPitchYaw(xRotation, yRotation, 0.0f));

	//InputManager::GetInstance().ReadControllerInputs();
	//std::array leftStick = { 0,0 };
	//std::array rightStick = { 0,0 };
	
	//float sensitivity = 2.0f;
	//float rotSpeed = sensitivity * Time::GetInstance().GetDeltaTime();
	 
	//rot[0] += rightStick.at(1) * rotSpeed * -1.0f;
	//rot[1] += rightStick.at(0) * rotSpeed;
	
	//if (rot[0] > 1.5f) rot[0] = 1.5f;
	//if (rot[0] < -1.5f) rot[0] = -1.5f;
	
	
	//this->transform.SetRotationRPY(0.0f, rot[0], rot[1]);
	
	//this->transform.Move(this->transform.GetDirectionVector(), Time::GetInstance().GetDeltaTime() * leftStick.at(1) * 20);
}
