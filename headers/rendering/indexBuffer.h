#pragma once

#include <cstdint>
#include <d3d11.h>
#include <wrl\client.h>
#include <format>

class IndexBuffer
{
private:
	Microsoft::WRL::ComPtr<ID3D11Buffer> buffer;
	size_t nrOfIndices = 0;

public:
	IndexBuffer() = default;
	~IndexBuffer() = default;
	IndexBuffer(const IndexBuffer& other) = delete;
	IndexBuffer& operator=(const IndexBuffer& other) = delete;
	IndexBuffer(IndexBuffer&& other) noexcept = default;
	IndexBuffer& operator=(IndexBuffer&& other) = delete;

	void Init(ID3D11Device* device, size_t nrOfIndicesInBuffer, uint32_t* indexData);

	size_t GetNrOfIndices() const;
	ID3D11Buffer* GetBuffer() const;
};