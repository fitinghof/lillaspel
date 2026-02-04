#pragma once

#include "gameObjects/gameObject.h"
#include "gameObjects/transform.h"

class GameObject3D : public GameObject {
public:
	GameObject3D();
	virtual ~GameObject3D() = default;

	virtual void Tick() override;

	Transform transform;

	DirectX::XMVECTOR GetGlobalPosition() const override;
	DirectX::XMMATRIX GetGlobalWorldMatrix(bool inverseTranspose) const override;

	virtual void LoadFromJson(const nlohmann::json& data) override;
	virtual nlohmann::json SaveToJson() override;
};