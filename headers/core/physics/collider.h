#pragma once
#include <memory>
#include "gameObjects/gameObject3D.h"
#include "core/physics/physics.h"
#include "core/physics/ray.h"

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
class RigidBody;

class Collider : public GameObject3D
{
public:
	Collider();
	~Collider();

	float shortestExtent = 1;

	virtual void Tick() override;
	void SetId(int id);
	int GetId();

	void ResolveCollision(DirectX::XMFLOAT3 resolveAxis, float resolveDistance);
	bool Collision(Collider* other);
	bool Collision(Collider* other, DirectX::XMVECTOR& contactNormal);

	void SetParent(std::weak_ptr<GameObject> newParent) override;

	virtual bool DoubleDispatchCollision(Collider* otherCollider, DirectX::XMFLOAT3& mtvAxis, float& mtvDistance) = 0;
	virtual bool CollidesWithBox(BoxCollider* box, DirectX::XMFLOAT3& resolveAxis, float& resolveDistance) = 0;
	virtual bool CollidesWithSphere(SphereCollider* sphere, DirectX::XMFLOAT3& resolveAxis, float& resolveDistance) = 0;
	virtual void SetPosition(DirectX::XMFLOAT3 newPosition) = 0;
	bool BoxSphereCollision(BoxCollider* box, SphereCollider* sphere, DirectX::XMFLOAT3& resolveAxis, float& resolveDistance);

	//DirectX::XMFLOAT3 resolveAxis = {};
	//float resolveDistance = 0;

	ColliderType type = ColliderType::NONE;
	Tag tag = Tag::OBJECT;
	Tag targetTag = Tag::OBJECT;
	PhysicsMaterial physicsMaterial;
	//DirectX::XMFLOAT3 previousPosition = {};
	bool solid = true;
	bool dynamic = false;
	bool hasInitializedPreviousPosition = false;

	/// <summary>
	/// returns true if ray intersects object with distance 
	/// to hit and max ray distance 
	/// </summary>
	/// <param name="ray"></param>
	/// <param name="distance"></param>
	/// <returns></returns>
	virtual bool IntersectWithRay(const Ray& ray, float& distance, float maxDistance) = 0;

private:
	int id = -1;
	std::weak_ptr<GameObject> meshObjectChild; //reference to the mesh visual representation of the collider (remove?)
	std::weak_ptr<RigidBody> castedParent;

	bool CollisionHandling(Collider* otherCollider, DirectX::XMFLOAT3& mtvAxis, float& mtvDistance);
};