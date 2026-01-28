#pragma once

#include "core/window.h"
#include "d3d11.h"
#include "wrl/client.h"
#include "rendering/renderTarget.h"
#include "rendering/depthBuffer.h"
#include "rendering/shader.h"
#include "rendering/inputLayout.h"
#include "rendering/sampler.h"

class Renderer {
public:
	Renderer() = default;
	~Renderer() = default;

	void Init(const Window& window);

	/// <summary>
	/// Render a frame and show it on the window
	/// </summary>
	void Render();
private:
	D3D11_VIEWPORT viewport;

	Microsoft::WRL::ComPtr<ID3D11Device> device;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> immediateContext;
	Microsoft::WRL::ComPtr<IDXGISwapChain> swapChain;

	std::unique_ptr<RenderTarget> renderTarget;
	std::unique_ptr<DepthBuffer> depthBuffer;
	std::unique_ptr<InputLayout> inputLayout;
	std::unique_ptr<Sampler> sampler;

	// Temp
	std::unique_ptr<Shader> vertexShader;
	std::unique_ptr<Shader> pixelShader;
	// -- 

	void SetViewport(const Window& window);
	void CreateDeviceAndSwapChain(const Window& window);
	void CreateRenderTarget();
	void CreateDepthBuffer(const Window& window);
	void CreateInputLayout(const std::string& vShaderByteCode);
	void CreateSampler();

	void LoadShaders(const std::string& vShaderByteCode);

	void RenderPass();
};