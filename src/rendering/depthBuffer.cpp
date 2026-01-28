#include "rendering/depthBuffer.h"

void DepthBuffer::Init(ID3D11Device* device, UINT width, UINT height)
{
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
	if (FAILED(device->CreateTexture2D(&textureDesc, nullptr, this->texture.GetAddressOf())))
	{
		throw std::exception("Failed to create Depth Stencil Texture!");
	}

	this->depthStencilViews.push_back(nullptr);
	int arrayIndex = this->depthStencilViews.size() - 1;
	HRESULT hr = device->CreateDepthStencilView(this->texture.Get(), 0, &this->depthStencilViews[arrayIndex]);
	if (FAILED(hr)) {
		throw std::exception("Error creating Depth Stencil! Error:" + hr);
	}
}

ID3D11DepthStencilView* DepthBuffer::GetDepthStencilView(UINT arrayIndex) const
{
	return this->depthStencilViews[arrayIndex].Get();
}
