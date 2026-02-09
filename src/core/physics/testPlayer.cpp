#include "core/physics/testPlayer.h"

TestPlayer::TestPlayer() {}

TestPlayer::~TestPlayer() {}

void TestPlayer::Tick()
{ 
	this->BoxCollider::Tick();
	DirectX::XMFLOAT3 oldPos;
	DirectX::XMStoreFloat3(&oldPos, this->transform.GetPosition());

	if (GetAsyncKeyState('W'))
	{
		this->SetPosition(FLOAT3ADD(oldPos, DirectX::XMFLOAT3(0, 0, 1)));
	}

	if (GetAsyncKeyState('A'))
	{
		this->SetPosition(FLOAT3ADD(oldPos, DirectX::XMFLOAT3(-1, 0, 0)));
	}

	if (GetAsyncKeyState('S'))
	{
		this->SetPosition(FLOAT3ADD(oldPos, DirectX::XMFLOAT3(0, 0, -1)));
	}

	if (GetAsyncKeyState('D'))
	{
		this->SetPosition(FLOAT3ADD(oldPos, DirectX::XMFLOAT3(1, 0, 0)));
	}

	// std::vector<std::weak_ptr<GameObject>> children = this->GetChildren();
	// std::shared_ptr<BoxCollider> box = children[0].lock(); ?????????????????????
}

void TestPlayer::Start()
{
}
