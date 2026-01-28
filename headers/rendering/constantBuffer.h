#pragma once

#include <d3d11.h>
#include <wrl/client.h>
#include <format>
#include <string>
#include <iostream>

class ConstantBuffer
{
private:
	Microsoft::WRL::ComPtr<ID3D11Buffer> buffer;
	size_t bufferSize = 0;

public:
	ConstantBuffer() = default;
	~ConstantBuffer() = default;
	ConstantBuffer(const ConstantBuffer& other) = delete;
	ConstantBuffer& operator=(const ConstantBuffer& other) = delete;

	void Init(ID3D11Device* device, size_t byteSize, void* initialData, D3D11_USAGE usage, UINT cpuAccess);

	size_t GetSize() const;
	ID3D11Buffer* GetBuffer() const;

	void UpdateBuffer(ID3D11DeviceContext* context, void* data);
};