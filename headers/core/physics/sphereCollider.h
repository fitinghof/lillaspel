#pragma once
#include <d3d11.h>
#include <DirectXMath.h>
#include "core/physics/collider.h"

//sphere collider transform scale has to be unifrom!

class BoxCollider;

class SphereCollider : public Collider
{
public:
	SphereCollider();
	~SphereCollider();

	bool DoubleDispatchCollision(Collider* otherCollider, DirectX::XMFLOAT3& mtvAxis, float& mtvDistance) override;

	void SetPosition(DirectX::XMFLOAT3 newCenter) override;
	void SetRotation(DirectX::XMFLOAT3 newRotation);
	void SetDiameter(float diameter);
	float GetDiameter();

private:
	bool CollidesWithBox(BoxCollider* box, DirectX::XMFLOAT3& resolveAxis, float& resolveDistance) override;
	bool CollidesWithSphere(SphereCollider* sphere, DirectX::XMFLOAT3& resolveAxis, float& resolveDistance) override;
};