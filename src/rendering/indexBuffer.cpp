#include "rendering/indexBuffer.h"

void IndexBuffer::Init(ID3D11Device* device, size_t nrOfIndicesInBuffer, uint32_t* indexData)
{
	D3D11_BUFFER_DESC bufferDesc;
	bufferDesc.ByteWidth = sizeof(uint32_t) * nrOfIndicesInBuffer;
	bufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
	bufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	bufferDesc.CPUAccessFlags = 0;
	bufferDesc.MiscFlags = 0;
	bufferDesc.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA data;
	data.pSysMem = indexData;
	data.SysMemPitch = 0;
	data.SysMemSlicePitch = 0;

	HRESULT hr = device->CreateBuffer(&bufferDesc, &data, &this->buffer);
	if (FAILED(hr))
	{
		throw std::exception(std::format("Failed to create index buffer, HRESULT: 0x{:08X}", static_cast<unsigned long>(hr)).c_str());
	}

	this->nrOfIndices = nrOfIndicesInBuffer;
}

size_t IndexBuffer::GetNrOfIndices() const
{
	return this->nrOfIndices;
}

ID3D11Buffer* IndexBuffer::GetBuffer() const
{
	return this->buffer.Get();
}