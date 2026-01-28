#include "rendering/inputLayout.h"

void InputLayout::AddInputElement(const std::string& semanticName, DXGI_FORMAT format)
{
	for (size_t i = 0; i < this->semanticNames.size(); i++)
	{
		if (this->semanticNames[i] == semanticName) {
			throw std::exception("Failed to add InputElement: Semantic already exist.");
		}
	}

	D3D11_INPUT_ELEMENT_DESC input = { semanticName.c_str(), 0, format, 0, (UINT)this->next_pos, D3D11_INPUT_PER_VERTEX_DATA, 0 };
	this->elements.push_back(input);

	size_t size = 0;
	switch (format) {
	case DXGI_FORMAT_R32G32B32_FLOAT:
		size = 12;
		break;
	case DXGI_FORMAT_R32G32_FLOAT:
		size = 8;
		break;
	}

	this->next_pos += size;
	semanticNames.push_back(semanticName);
}

void InputLayout::FinalizeInputLayout(ID3D11Device* device, const void* vsDataPtr, size_t vsDataSize)
{
	for (size_t i = 0; i < this->elements.size(); i++)
	{
		this->elements[i].SemanticName = this->semanticNames[i].c_str();
	}

	if (vsDataPtr == nullptr) {
		throw std::exception("Failed to create input layout: vsDataPtr is null");
	}

	HRESULT hr = device->CreateInputLayout(this->elements.data(), this->elements.size(), vsDataPtr, vsDataSize, this->inputLayout.GetAddressOf());

	if (FAILED(hr)) {
		throw std::exception("Failed to create input layout");
	}
}

ID3D11InputLayout* InputLayout::GetInputLayout() const
{
	return this->inputLayout.Get();
}