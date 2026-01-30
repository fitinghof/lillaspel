#include "rendering/rasterizerState.h"

void RasterizerState::Init(ID3D11Device* device, const D3D11_RASTERIZER_DESC* desc)
{
	Logger::Log(this->rasterizerState.Get());
	Logger::Log(desc);

	HRESULT hr = device->CreateRasterizerState(desc, this->rasterizerState.GetAddressOf());
	if (FAILED(hr)) {
		throw std::runtime_error(std::format("Failed to create rasterizer,  HRESULT: 0x{:08X}", static_cast<unsigned long>(hr)).c_str());
	}

	Logger::Log(this->rasterizerState.Get());
}

ID3D11RasterizerState* RasterizerState::GetRasterizerState()
{
	return this->rasterizerState.Get();
}
