#include "rendering/renderTarget.h"

void RenderTarget::Init(ID3D11Device* device, IDXGISwapChain* swapChain)
{
	// get the address of the back buffer
	ID3D11Texture2D* backBuffer = nullptr;
	if (FAILED(swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&backBuffer))))
	{
		throw std::exception("Failed to get backbuffer!");
		return;
	}

	// use the back buffer address to create the render target
	// null as description to base it on the backbuffers values
	HRESULT hr = device->CreateRenderTargetView(backBuffer, NULL, this->renderTargetView.GetAddressOf());
	backBuffer->Release();
	if (FAILED(hr)) {
		throw std::exception(std::format("Error creating render target view, HRESULT: 0x{:08X}", static_cast<unsigned long>(hr)).c_str());
	}
}

ID3D11RenderTargetView* RenderTarget::GetRenderTargetView()
{
	return this->renderTargetView.Get();
}

ID3D11ShaderResourceView* RenderTarget::GetShaderResourceView() const
{
	return this->shaderResourceView.Get();
}
