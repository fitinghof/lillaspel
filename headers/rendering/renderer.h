#pragma once

#include "core/window.h"
#include "d3d11.h"
#include "wrl/client.h"

class Renderer {
public:
	Renderer() = default;
	~Renderer() = default;

	void Init(const Window& window);
private:
	D3D11_VIEWPORT viewport;

	Microsoft::WRL::ComPtr<ID3D11Device> device;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> immediateContext;
	Microsoft::WRL::ComPtr<IDXGISwapChain> swapChain;

	void SetViewport(const Window& window);
	void CreateDeviceAndSwapChain(const Window& window);
};