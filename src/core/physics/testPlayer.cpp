#include "core/physics/testPlayer.h"

TestPlayer::TestPlayer() {}

TestPlayer::~TestPlayer() {}

void TestPlayer::Tick()
{ 
	this->GameObject3D::Tick();
	DirectX::XMFLOAT3 oldPos;
	DirectX::XMStoreFloat3(&oldPos, this->transform.GetPosition());
	float deltaTime = Time::GetInstance().GetDeltaTime();

	DirectX::XMFLOAT3 moveVector = DirectX::XMFLOAT3(0, 0, 0);
	float speed = 5;

	if (GetAsyncKeyState('I'))
	{
		moveVector = FLOAT3ADD(moveVector, DirectX::XMFLOAT3(0, 0, speed));
	}

	if (GetAsyncKeyState('J'))
	{
		moveVector = FLOAT3ADD(moveVector, DirectX::XMFLOAT3(-speed, 0, 0));
	}

	if (GetAsyncKeyState('K'))
	{
		moveVector = FLOAT3ADD(moveVector, DirectX::XMFLOAT3(0, 0, -speed));
	}

	if (GetAsyncKeyState('L'))
	{
		moveVector = FLOAT3ADD(moveVector, DirectX::XMFLOAT3(speed, 0, 0));
	}

	DirectX::XMFLOAT3 newPos = FLOAT3ADD(oldPos, FLOAT3MULT1(moveVector, deltaTime));
	this->transform.SetPosition(DirectX::XMLoadFloat3(&newPos));

	// std::vector<std::weak_ptr<GameObject>> children = this->GetChildren();
	// std::shared_ptr<BoxCollider> box = children[0].lock(); ?????????????????????
}

void TestPlayer::Start()
{
}
