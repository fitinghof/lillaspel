#include "gameObjects/debugCamera.h"

void DebugCamera::Tick()
{
	this->CameraObject::Tick();

	// Until we get input


	int centerX = 500;
	int centerY = 500;
	POINT cursorPos;
	GetCursorPos(&cursorPos);
	// Logic to calculate delta and rotate
	
	


	//static float pos[3] = { 0,0,0 };
	static float rot[3] = { 0,0,0 };


	InputManager::GetInstance().ReadControllerInputs();
	std::array leftStick = InputManager::GetInstance().GetLeftThumbMovement();
	std::array rightStick = InputManager::GetInstance().GetRightThumbMovement();

	float sensitivity = 2.0f;
	float rotSpeed = sensitivity * Time::GetInstance().GetDeltaTime();

	rot[0] += rightStick.at(1) * rotSpeed * -1.0f * (cursorPos.y - centerY);
	rot[1] += rightStick.at(0) * rotSpeed * (cursorPos.x - centerX);

	if (rot[0] > 1.5f) rot[0] = 1.5f;
	if (rot[0] < -1.5f) rot[0] = -1.5f;


	this->transform.SetRotationRPY(0.0f, rot[0], rot[1]);

	this->transform.Move(this->transform.GetDirectionVector(), Time::GetInstance().GetDeltaTime() * leftStick.at(1) * 20);

	SetCursorPos(centerX, centerY);
}
