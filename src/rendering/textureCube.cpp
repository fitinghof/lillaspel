#include "textureCube.h"

void TextureCube::CreateTextures(ID3D11Device* device, UINT width, UINT height) 
{
	D3D11_TEXTURE2D_DESC desc;
	desc.Width = width;
	desc.Height = height;
	desc.MipLevels = 1;
	desc.ArraySize = 6;
	desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM; 
	desc.SampleDesc = {1, 0};				  // Create new sampleDesc with Count = 1, Quality = 0
	desc.Usage = D3D11_USAGE::D3D11_USAGE_DEFAULT;
	desc.BindFlags = D3D11_BIND_UNORDERED_ACCESS | D3D11_BIND_SHADER_RESOURCE;
	desc.CPUAccessFlags = 0;
	desc.MiscFlags = D3D11_RESOURCE_MISC_TEXTURECUBE;

	HRESULT hr = device->CreateTexture2D(&desc, nullptr, &this->textures);
	if (FAILED(hr)) {
		throw std::exception("Failed to create texture cube!");
	}
}
