#pragma once
#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include "core/window.h"
#include "d3d11.h"
#include "wrl/client.h"
#include "rendering/renderTarget.h"
#include "rendering/depthBuffer.h"
#include "rendering/shader.h"
#include "rendering/inputLayout.h"
#include "rendering/sampler.h"
#include "rendering/vertex.h"
#include "rendering/vertexBuffer.h"
#include "rendering/constantBuffer.h"
#include "rendering/indexBuffer.h"
#include "rendering/material.h"
#include "rendering/renderQueue.h"

#include "rendering/tempRenderDefs.h"

class Renderer {
public:
	Renderer() = default;
	~Renderer() = default;

	/// <summary>
	/// Initialize the renderer
	/// </summary>
	/// <param name="window"></param>
	void Init(const Window& window);

	/// <summary>
	/// Render a frame
	/// </summary>
	void Render();

	/// <summary>
	/// Present the swapchain to the window
	/// </summary>
	void Present();

	ID3D11Device* GetDevice() const;
	ID3D11DeviceContext* GetContext() const;
	IDXGISwapChain* GetSwapChain() const;
private:
	D3D11_VIEWPORT viewport;

	Microsoft::WRL::ComPtr<ID3D11Device> device;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> immediateContext;
	Microsoft::WRL::ComPtr<IDXGISwapChain> swapChain;

	std::unique_ptr<RenderTarget> renderTarget;
	std::unique_ptr<DepthBuffer> depthBuffer;
	std::unique_ptr<InputLayout> inputLayout;
	std::unique_ptr<Sampler> sampler;

	// Temporary

	std::unique_ptr<Material> tempMat;

	std::shared_ptr<Shader> vertexShader;
	std::shared_ptr<Shader> pixelShader;

	// -- 

	std::unique_ptr<RenderQueue> renderQueue;
	std::shared_ptr<std::vector<int>> meshRenderQueue;

	void SetViewport(const Window& window);
	void CreateDeviceAndSwapChain(const Window& window);
	void CreateRenderTarget();
	void CreateDepthBuffer(const Window& window);
	void CreateInputLayout(const std::string& vShaderByteCode);
	void CreateSampler();

	void CreateRenderQueue();

	void LoadShaders(std::string& vShaderByteCode);

	/// <summary>
	/// This is where the actual rendering logic is done
	/// </summary>
	void RenderPass();

	/// <summary>
	/// Clears last frame with a clear color
	/// </summary>
	void ClearRenderTargetViewAndDepthStencilView();

	void BindSampler();
	void BindInputLayout();
	void BindRenderTarget();
	void BindViewport();

	void BindMaterial(Material* material);

	void BindCameraMatrix(ID3D11Buffer* buffer);
	void BindWorldMatrix(ID3D11Buffer* buffer);
};