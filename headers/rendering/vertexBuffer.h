#pragma once

#include <d3d11.h>
#include <iostream>
#include <wrl/client.h>

class VertexBuffer
{
private:
	Microsoft::WRL::ComPtr<ID3D11Buffer> buffer = nullptr;
	UINT nrOfVertices = 0;
	UINT vertexSize = 0;

public:
	VertexBuffer() = default;
	~VertexBuffer() = default;
	VertexBuffer(const VertexBuffer& other) = delete;
	VertexBuffer& operator=(const VertexBuffer& other) = delete;
	VertexBuffer(VertexBuffer&& other) = delete;
	VertexBuffer& operator=(VertexBuffer&& other) = delete;

	void Init(ID3D11Device* device, UINT sizeOfVertex,
		UINT nrOfVerticesInBuffer, void* vertexData);

	UINT GetNrOfVertices() const;
	UINT GetVertexSize() const;
	ID3D11Buffer* GetBuffer() const;
};