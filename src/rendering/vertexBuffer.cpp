#include "vertexBuffer.h"

void VertexBuffer::Init(ID3D11Device* device, UINT sizeOfVertex, UINT nrOfVerticesInBuffer, void* vertexData)
{
	D3D11_BUFFER_DESC bufferDesc;
	bufferDesc.ByteWidth = sizeOfVertex * nrOfVerticesInBuffer;
	bufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
	bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bufferDesc.CPUAccessFlags = 0;
	bufferDesc.MiscFlags = 0;
	bufferDesc.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA data;
	data.pSysMem = vertexData;
	data.SysMemPitch = 0;
	data.SysMemSlicePitch = 0;

	HRESULT hr = device->CreateBuffer(&bufferDesc, &data, this->buffer.GetAddressOf());
	if (FAILED(hr)) {
		throw std::exception("Failed to create vertex buffer.");
	}

	this->vertexSize = sizeOfVertex;
	this->nrOfVertices = nrOfVerticesInBuffer;
}

UINT VertexBuffer::GetNrOfVertices() const
{
	return this->nrOfVertices;
}

UINT VertexBuffer::GetVertexSize() const
{
	return this->vertexSize;
}

ID3D11Buffer* VertexBuffer::GetBuffer() const
{
	return this->buffer.Get();
}
