#include "gameObjects/debugCamera.h"

#include "core/input/inputManager.h"
#include "utilities/time.h"
#include "utilities/logger.h"

void DebugCamera::Tick()
{
	this->CameraObject::Tick();


	//static float pos[3] = { 0,0,0 };
	static float rot[3] = { 0,0,0 };

	if (keyboardInput.Quit()) {
		PostQuitMessage(0);
	}

	/*if (keyboardInput.Jump()) {
		Logger::Log("Jump pressed");
	}
	
	std::array movementVector = keyboardInput.GetMovementVector();
	if (movementVector[0] != 0.0f || movementVector[1] != 0.0f) {
		Logger::Log("Movement vector: " + std::to_string(movementVector[0]) + ", " + std::to_string(movementVector[1]));
	}*/

	std::array lookVector = keyboardInput.GetLookVector();
	if (lookVector[0] != 0.0f || lookVector[1] != 0.0f)
		Logger::Log("Look vector: " + std::to_string(lookVector[0]) + ", " + std::to_string(lookVector[1]));
	
	/*InputManager::GetInstance().ReadControllerInputs();
	std::array leftStick = { 0,0 };
	std::array rightStick = { 0,0 };

	float sensitivity = 2.0f;
	float rotSpeed = sensitivity * Time::GetInstance().GetDeltaTime();
	 
	rot[0] += rightStick.at(1) * rotSpeed * -1.0f;
	rot[1] += rightStick.at(0) * rotSpeed;

	if (rot[0] > 1.5f) rot[0] = 1.5f;
	if (rot[0] < -1.5f) rot[0] = -1.5f;


	this->transform.SetRotationRPY(0.0f, rot[0], rot[1]);

	this->transform.Move(this->transform.GetDirectionVector(), Time::GetInstance().GetDeltaTime() * leftStick.at(1) * 20);*/
}
