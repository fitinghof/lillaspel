#pragma once

#include "core/window.h"
#include "d3d11.h"
#include "wrl/client.h"
#include "rendering/renderTarget.h"
#include "rendering/depthBuffer.h"

class Renderer {
public:
	Renderer() = default;
	~Renderer() = default;

	void Init(const Window& window);

	void Render();
private:
	D3D11_VIEWPORT viewport;

	Microsoft::WRL::ComPtr<ID3D11Device> device;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> immediateContext;
	Microsoft::WRL::ComPtr<IDXGISwapChain> swapChain;

	std::unique_ptr<RenderTarget> renderTarget;
	std::unique_ptr<DepthBuffer> depthBuffer;

	void SetViewport(const Window& window);
	void CreateDeviceAndSwapChain(const Window& window);
	void CreateRenderTarget();
	void CreateDepthBuffer(const Window& window);

	void RenderPass();
};