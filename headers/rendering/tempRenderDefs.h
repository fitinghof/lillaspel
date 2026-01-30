#pragma once
#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <DirectXMath.h>

// This entire file exists only to simplify temporary logic in renderer.cpp
// Hopefully we will have classes doing this in the future



struct MatrixContainer {
	DirectX::XMFLOAT4X4 matrix;
};

struct CameraBufferContainer {
	MatrixContainer matrixBuffer;
	float camPos[4];
};

struct WorldMatrixBufferContainer {
	MatrixContainer worldMatrix;
	MatrixContainer worldMatrixInversedTransposed;
};


static DirectX::XMMATRIX XMMATRIX_ProjMatrix_Perspective(float fov, float aspectRatio)
{
	DirectX::XMMATRIX projMatrix = DirectX::XMMatrixPerspectiveFovLH(DirectX::XMConvertToRadians(fov), aspectRatio, 0.1f, 1000.0f);
	return projMatrix;
}

static DirectX::XMMATRIX XMMATRIX_ViewMatrix(float* cameraPos, float* lookPos, float* up)
{
	DirectX::XMVECTOR eyePos = DirectX::XMVectorSet(cameraPos[0], cameraPos[1], cameraPos[2], 1.0f);
	DirectX::XMVECTOR focusPos = DirectX::XMVectorSet(lookPos[0], lookPos[1], lookPos[2], 1.0f);
	DirectX::XMVECTOR upDir = DirectX::XMVectorSet(up[0], up[1], up[2], 1.0f);
	DirectX::XMMATRIX viewMatrix = DirectX::XMMatrixLookAtLH(eyePos, focusPos, upDir);

	return viewMatrix;
}


static DirectX::XMMATRIX XMMATRIX_ViewProjMatrix_Perspective(float fov, float aspectRatio, float* cameraPos, float* lookPos, float* up)
{
	return XMMATRIX_ViewMatrix(cameraPos, lookPos, up) * XMMATRIX_ProjMatrix_Perspective(fov, aspectRatio);
}

static void ConstantBufferViewProjMatrix_Perspective(MatrixContainer*& data, float fov, float aspectRatio, float* cameraPos, float* lookPos, float* up)
{
	DirectX::XMMATRIX viewProjMatrix = /*DirectX::XMMatrixTranspose(*/XMMATRIX_ViewProjMatrix_Perspective(fov, aspectRatio, cameraPos, lookPos, up)/*)*/;

	MatrixContainer* vpMat = new MatrixContainer();
	DirectX::XMStoreFloat4x4(&vpMat->matrix, viewProjMatrix);

	data = vpMat;
}

// SHould maybe use Quaternions in the non-temp version?
static DirectX::XMMATRIX XMMATRIX_WorldMatrix(float* pos, float* rot, float* scale, bool inverseTranspose = false)
{
	DirectX::XMMATRIX translatedMatrix = DirectX::XMMatrixTranslation(pos[0], pos[1], pos[2]);
	DirectX::XMMATRIX rotationMatrix = DirectX::XMMatrixRotationX(DirectX::XMConvertToRadians(rot[0])) * DirectX::XMMatrixRotationY(DirectX::XMConvertToRadians(rot[1])) * DirectX::XMMatrixRotationZ(DirectX::XMConvertToRadians(rot[2]));
	DirectX::XMMATRIX scaledMatrix = DirectX::XMMatrixScaling(scale[0], scale[1], scale[2]);
	DirectX::XMMATRIX worldMatrix = scaledMatrix * rotationMatrix * translatedMatrix;

	if (inverseTranspose) {
		worldMatrix = DirectX::XMMatrixInverse(nullptr, worldMatrix);
		worldMatrix = DirectX::XMMatrixTranspose(worldMatrix);
	}

	// From row-major to column-major
	//worldMatrix = DirectX::XMMatrixTranspose(worldMatrix);

	return worldMatrix;
}

static void ConstantBufferWorldMatrix(MatrixContainer*& data, float* pos, float* rot, float* scale, bool inverseTranspose = false)
{
	DirectX::XMMATRIX worldMatrix = XMMATRIX_WorldMatrix(pos, rot, scale, inverseTranspose);
	MatrixContainer* wMat = new MatrixContainer();
	DirectX::XMStoreFloat4x4(&wMat->matrix, worldMatrix);

	data = wMat;
}
