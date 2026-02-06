#pragma once

#define NOMINMAX
#define WIN32_LEAN_AND_MEAN

#include "gameObjects/gameObject3D.h"
#include <DirectXMath.h>
#include "utilities/logger.h"

#include "imgui.h"
#include <string>

class CameraObject : public GameObject3D {
public:
	/// <summary>
	/// For inputting into a constant buffer
	/// </summary>
	struct CameraMatrixContainer {
		/// <summary>
		/// View Projection Matrix of the camera
		/// </summary>
		DirectX::XMFLOAT4X4 viewProjectionMatrix;
		/// <summary>
		/// Camera world position
		/// </summary>
		DirectX::XMVECTOR cameraPosition; 
	};

	CameraObject();
	virtual ~CameraObject() = default;

	virtual void Tick() override;
	virtual void LateTick() override;

	CameraObject::CameraMatrixContainer& GetCameraMatrix();
	static CameraObject& GetMainCamera();

	virtual void LoadFromJson(const nlohmann::json& data) override;
	virtual void SaveToJson(nlohmann::json& data) override;

private:
	static CameraObject* mainCamera;
	static size_t cameraId;
	size_t thisCameraId;

	CameraMatrixContainer cameraMatrix;

	/// <summary>
	/// Field of view (FOV) in degrees
	/// </summary>
	float fieldOfView;

	void UpdateCameraMatrix();
};