#include "rendering/depthBuffer.h"

void DepthBuffer::Init(ID3D11Device* device, UINT width, UINT height)
{

	if (this->texture.Get()) this->texture.Reset();
	if (this->shaderResourceView.Get()) this->shaderResourceView.Reset();
	if (this->depthStencilViews.Get()) this->depthStencilViews.Reset();
	if (this->depthStencilState.Get()) this->depthStencilState.Reset();

	D3D11_DEPTH_STENCIL_DESC depthStencilDesc;
	depthStencilDesc.DepthEnable = true;
	depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;
	depthStencilDesc.StencilEnable = false;
	depthStencilDesc.StencilReadMask = 0;
	depthStencilDesc.StencilWriteMask = 0;

	HRESULT hr0 = device->CreateDepthStencilState(&depthStencilDesc, this->depthStencilState.GetAddressOf());
	if (FAILED(hr0)) {
		std::string output =
			std::format("Failed to create Depth Stencil State, HRESULT: 0x{:08X}", static_cast<unsigned long>(hr0));
		Logger::Error(output);
		throw std::exception(output.c_str());
	}

	D3D11_TEXTURE2D_DESC textureDesc;
	textureDesc.Width = width;
	textureDesc.Height = height;
	textureDesc.MipLevels = 1;
	textureDesc.ArraySize = 1;
	textureDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	textureDesc.SampleDesc.Count = 1;
	textureDesc.SampleDesc.Quality = 0;
	textureDesc.Usage = D3D11_USAGE_DEFAULT;
	textureDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	textureDesc.CPUAccessFlags = 0;
	textureDesc.MiscFlags = 0;


	HRESULT hr1 = device->CreateTexture2D(&textureDesc, nullptr, this->texture.GetAddressOf());
	if (FAILED(hr1))
	{
		throw std::exception(std::format("Failed to create Depth Stencil Texture, HRESULT: 0x{:08X}", static_cast<unsigned long>(hr1)).c_str());
	}

	HRESULT hr =
		device->CreateDepthStencilView(this->texture.Get(), 0, this->depthStencilViews.GetAddressOf());
	if (FAILED(hr)) {
		throw std::exception(std::format("Error creating Depth Stencil, HRESULT: 0x{:08X}", static_cast<unsigned long>(hr)).c_str());
	}
}


[[deprecated]] ID3D11DepthStencilView* DepthBuffer::GetDepthStencilView(UINT arrayIndex) const {
	return this->depthStencilViews.Get();
}

ID3D11DepthStencilView* DepthBuffer::GetDepthStencilView() const {
	return this->depthStencilViews.Get();
}

ID3D11ShaderResourceView* DepthBuffer::GetShaderResourceView() const
{
	return this->shaderResourceView.Get(); }

ID3D11DepthStencilState* DepthBuffer::GetDepthStencilState() const
{ 
	return this->depthStencilState.Get();
}
