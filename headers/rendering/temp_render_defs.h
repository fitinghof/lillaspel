#include <DirectXMath.h>

struct MatrixContainer {
	DirectX::XMFLOAT4X4 matrix;
};

struct CameraBufferContainer {
	MatrixContainer matrixBuffer;
	float camPos[4];
};

//DirectX::XMMATRIX XMMATRIX_ViewProjMatrix_Perspective(float fov, float aspectRatio, Vector3 cameraPos, Vector3 lookPos, Vector3 up)
//{
//	return XMMATRIX_ViewMatrix(cameraPos, lookPos, up) * XMMATRIX_ProjMatrix_Perspective(fov, aspectRatio);
//}
//
//void ConstantBufferViewProjMatrix_Perspective(MatrixContainer*& data, float fov, float aspectRatio, Vector3 cameraPos, Vector3 lookPos, Vector3 up)
//{
//	DirectX::XMMATRIX viewProjMatrix = DirectX::XMMatrixTranspose(XMMATRIX_ViewProjMatrix_Perspective(fov, aspectRatio, cameraPos, lookPos, up));
//
//	MatrixContainer* vpMat = new MatrixContainer();
//	DirectX::XMStoreFloat4x4(&vpMat->matrix, viewProjMatrix);
//
//	data = vpMat;
//}
