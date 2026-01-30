#pragma once

#include "gameObjects/gameObject3D.h"

#include <DirectXMath.h>

#include "utilities/logger.h"

#include "imgui.h"

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

	static CameraObject* mainCamera;

	CameraObject();
	virtual ~CameraObject() = default;

	virtual void Tick() override;

	CameraObject::CameraMatrixContainer& GetCameraMatrix();

private:
	CameraMatrixContainer cameraMatrix;

	/// <summary>
	/// Field of view (FOV) in degrees
	/// </summary>
	float fieldOfView;

	void UpdateCameraMatrix();
};