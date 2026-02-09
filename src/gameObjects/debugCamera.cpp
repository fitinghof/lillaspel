#include "gameObjects/debugCamera.h"
#include "imgui.h"
#include "core/input/inputManager.h"
#include "utilities/time.h"
#include "core/assetManager.h"
#include "gameObjects/meshObject.h"
void DebugCamera::Tick()
{
	this->CameraObject::Tick();

	// Until we get input
	InputManager::GetInstance().ReadControllerInputs();
	static bool useMouseMovment = false;
	int centerX = 1000;
	int centerY = 500;
	static bool mouseInput = false;
	static bool relesedSpace = true;

	if (!GetAsyncKeyState(VK_SPACE)) {
		relesedSpace = true;
	}

	if (GetAsyncKeyState(VK_SPACE) && relesedSpace)
	{
		relesedSpace = false;
		if (mouseInput)mouseInput = false;
		else
		{
			mouseInput = true;
		}

	}

	if (mouseInput) {
		POINT cursorPos;

		GetCursorPos(&cursorPos);

		static float rot[3] = { 0,0,0 };


		
		std::array rightStick = InputManager::GetInstance().GetRightThumbMovement();

		float sensitivity = 2.0f;
		float rotSpeed = sensitivity * Time::GetInstance().GetDeltaTime();
		
		rot[0] += rightStick.at(1) * rotSpeed * -1.0f * ((cursorPos.y - centerY) / 10);
		rot[1] += rightStick.at(0) * rotSpeed * ((cursorPos.x - centerX) / 10);

		rot[0] += rotSpeed *((cursorPos.y - centerY));
		rot[1] += rotSpeed * ((cursorPos.x - centerX));

		if (rot[0] > 1.5f) rot[0] = 1.5f;
		if (rot[0] < -1.5f) rot[0] = -1.5f;


		this->transform.SetRotationRPY(0.0f, rot[0], rot[1]);



		SetCursorPos(1000, 500);

	}
	//static float pos[3] = { 0,0,0 };
	std::array leftStick = InputManager::GetInstance().GetLeftThumbMovement();
	this->transform.Move(this->transform.GetDirectionVector(), Time::GetInstance().GetDeltaTime() * leftStick.at(1) * 20);

	if (GetAsyncKeyState('W'))
	{
		this->transform.Move(this->transform.GetDirectionVector(), Time::GetInstance().GetDeltaTime() *  15);
	}

	if (GetAsyncKeyState('S'))
	{
		this->transform.Move(this->transform.GetDirectionVector(), Time::GetInstance().GetDeltaTime() * -15);
	}

	static int test = 0;
	if (test < 10) {
		test++;
		Logger::Log("Test", test);
		auto meshObj = this->factory->CreateGameObjectOfType<MeshObject>();
		meshObj.lock()->SetMesh(AssetManager::GetInstance().GetMeshObjData("TexBox/TextureCube.glb:Mesh_0"));
		meshObj.lock()->transform.SetPosition(DirectX::XMVectorSet(test * 6, 0, 0, 0));
		//meshObj.lock()->SetParent(this->GetPtr());
	}

	
}
