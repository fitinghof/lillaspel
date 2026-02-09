#pragma once
#include <d3d11.h>
#include <DirectXMath.h>
#include "core/physics/collider.h"

//how will satData.center and BuildCornersArray be handled?

class SphereCollider;

class BoxCollider : public Collider
{
public:
	BoxCollider();
	~BoxCollider();

	void Tick() override;

	//Center and Size is only in the transform
	bool DoubleDispatchCollision(Collider* otherCollider, DirectX::XMFLOAT3& mtvAxis, float& mtvDistance) override;

	void SetPosition(DirectX::XMFLOAT3 newCenter) override;
	void SetRotation(DirectX::XMFLOAT3 newRotation);
	void SetExtents(DirectX::XMFLOAT3 newSize);
	DirectX::XMFLOAT3 GetExtents();

private:
	DirectX::XMFLOAT3 axis[3];
	SATData satData = {};

	bool CollidesWithBox(BoxCollider* box, DirectX::XMFLOAT3& resolveAxis, float& resolveDistance) override;
	bool CollidesWithSphere(SphereCollider* sphere, DirectX::XMFLOAT3& resolveAxis, float& resolveDistance) override;
	void BuildCornersArray(DirectX::XMFLOAT3*& positionArray);
};