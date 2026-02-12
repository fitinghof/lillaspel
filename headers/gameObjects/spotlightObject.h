#pragma once

#include "gameObjects/gameObject3D.h"
#include "rendering/renderQueue.h"
#include <DirectXMath.h>
#include "rendering/depthBuffer.h"
#include "gameObjects/cameraObject.h"

class SpotlightObject : public GameObject3D {
public:
	struct alignas(16) SpotLightContainer {
		DirectX::XMFLOAT3 position;	 // 12 b
		DirectX::XMFLOAT3 direction; // 12 b

		DirectX::XMFLOAT4 color; // 16 b
		float intensity;		 // 4 b

		float spotAngleRadians; // 4 b
	};



	SpotlightObject();
	~SpotlightObject() = default;


	SpotLightContainer GetSpotLightData() const;
	ID3D11DepthStencilView* GetDepthStencilView() const;

	virtual void Start() override;
	virtual void Tick() override;

	virtual void LoadFromJson(const nlohmann::json& data) override;
	virtual void SaveToJson(nlohmann::json& data) override;

	std::weak_ptr<CameraObject> camera;
	void SetShadowResolution(ID3D11Device* device, size_t width, size_t height);

private:



	D3D11_VIEWPORT shadowViewPort;
	SpotLightContainer data;
	DepthBuffer shadowbuffer;
};