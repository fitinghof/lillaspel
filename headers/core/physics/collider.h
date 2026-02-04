#pragma once
#define NOMINMAX //tells windows to not define min/max so I can use algorithm min/max instead
#include <algorithm>
#include "Physics.hpp"
#include "Transform.hpp"
#include "Shader.h"
#include "Tools.hpp"

static const DirectX::XMFLOAT3 localBoxCorners[8] =
{
	{ -1, -1, -1 },
	{ +1, -1, -1 },
	{ -1, +1, -1 },
	{ +1, +1, -1 },
	{ -1, -1, +1 },
	{ +1, -1, +1 },
	{ -1, +1, +1 },
	{ +1, +1, +1 }
};



enum Tag
{
	PLAYER,
	PLAYER_ATTACK,
	ENEMY,
	ENEMY_ATTACK,
	GROUND,
	FLOOR,
	OBJECT,
};

enum ColliderType
{
	BOX,
	SPHERE,
	NONE
};

class BoxCollider; //forward declaration
class SphereCollider;

class Collider
{
public:
	Collider();
	~Collider();

	Mesh* visualMesh = nullptr;
	Shader* pixelShader = nullptr;
	float shortestExtent = 1;

	void ResolveCollision(DirectX::XMFLOAT3 resolveAxis, float resolveDistance);
	bool Collision(Collider* other);
	bool Collision(Collider* other, DirectX::XMVECTOR& contactNormal);

	virtual void Physics() = 0;
	virtual bool DoubleDispatchCollision(Collider* otherCollider, DirectX::XMFLOAT3& mtvAxis, float& mtvDistance) = 0;
	virtual bool CollidesWithBox(BoxCollider* box, DirectX::XMFLOAT3& resolveAxis, float& resolveDistance) = 0;
	virtual bool CollidesWithSphere(SphereCollider* sphere, DirectX::XMFLOAT3& resolveAxis, float& resolveDistance) = 0;
	virtual void SetPosition(DirectX::XMFLOAT3 newPosition) = 0;
	bool BoxSphereCollision(BoxCollider* box, SphereCollider* sphere, DirectX::XMFLOAT3& resolveAxis, float& resolveDistance);

	DirectX::XMFLOAT3 GetPosition();
	DirectX::XMFLOAT3 GetSize();
	void MakeDrawable(ID3D11Device* device); //matrix buffers are created in scene for DrawColliders
	void DrawVisualMesh(ID3D11DeviceContext* immediateContext);

	//DirectX::XMFLOAT3 resolveAxis = {};
	//float resolveDistance = 0;

	ColliderType type = ColliderType::NONE;
	Tag tag = Tag::OBJECT;
	PhysicsMaterial physicsMaterial;
	Transform transform;
	DirectX::XMFLOAT3 previousPosition = {};
	bool solid = true;
	bool dynamic = false;
	bool hasInitializedPreviousPosition = false;

private:
	bool CollisionHandling(Collider* otherCollider, DirectX::XMFLOAT3& mtvAxis, float& mtvDistance);
};