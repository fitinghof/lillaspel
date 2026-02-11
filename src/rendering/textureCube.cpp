#include "rendering/textureCube.h"

void TextureCube::CreateTextures(ID3D11Device* device, ID3D11DeviceContext* deviceContext, std::string filepath) 
{
	const std::wstring wstr = std::wstring(filepath.begin(), filepath.end());
	DirectX::CreateDDSTextureFromFileEx(device, deviceContext, wstr.c_str(), 0,
										D3D11_USAGE_DEFAULT,
										D3D11_BIND_SHADER_RESOURCE, 0, D3D11_RESOURCE_MISC_TEXTURECUBE, DirectX::DX11::DDS_LOADER_DEFAULT,
										this->texture.GetAddressOf(), this->shaderResourceView.GetAddressOf());
}
