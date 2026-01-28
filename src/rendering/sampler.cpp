#include "rendering/sampler.h"

void Sampler::Init(ID3D11Device* device, D3D11_TEXTURE_ADDRESS_MODE adressMode)
{
	D3D11_SAMPLER_DESC samplerDesc;
	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR; // Uses linear interpolation
	samplerDesc.AddressU = adressMode; // Uses wrap for all sides
	samplerDesc.AddressV = adressMode;
	samplerDesc.AddressW = adressMode; // Is this even used for a 2D texture?
	samplerDesc.MipLODBias = 0; // Only have on miplevel, so doesn't really matter, but it's an offset from calculated mipmaplevel
	samplerDesc.MaxAnisotropy = 1; // Unused
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_NEVER; // Isn't really relevant for this usecase
	samplerDesc.MinLOD = 0;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

	samplerDesc.BorderColor[0] = 0;
	samplerDesc.BorderColor[1] = 0;
	samplerDesc.BorderColor[2] = 0;
	samplerDesc.BorderColor[3] = 1;

	HRESULT hr = device->CreateSamplerState(&samplerDesc, &sampler);

	if (FAILED(hr)) {
		throw std::exception("Failed to create the sampler state");
	}
}

ID3D11SamplerState* Sampler::GetSamplerState() const
{
	return this->sampler.Get();
}
