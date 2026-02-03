#pragma once

#define NOMINMAX
#define WIN32_LEAN_AND_MEAN

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
#include "rendering/rasterizerState.h"
#include "gameObjects/cameraObject.h"
#include "gameObjects/meshObject.h"




class Renderer {
public:
	Renderer();
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

	void Resize(const Window& window);

	void ToggleVSync(bool enable);

	ID3D11Device* GetDevice() const;
	ID3D11DeviceContext* GetContext() const;
	IDXGISwapChain* GetSwapChain() const;
private:

	struct WorldMatrixBufferContainer {
		DirectX::XMFLOAT4X4 worldMatrix;
		DirectX::XMFLOAT4X4 worldMatrixInversedTransposed;
	};

	// DirectX11 specific stuff:

	D3D11_VIEWPORT viewport;

	Microsoft::WRL::ComPtr<ID3D11Device> device;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> immediateContext;
	Microsoft::WRL::ComPtr<IDXGISwapChain> swapChain;

	std::unique_ptr<RenderTarget> renderTarget;
	std::unique_ptr<DepthBuffer> depthBuffer;
	std::unique_ptr<InputLayout> inputLayout;
	std::unique_ptr<Sampler> sampler;
	std::unique_ptr<RasterizerState> standardRasterizerState;


	// Temporary:

	std::unique_ptr<Material> tempMat;

	std::shared_ptr<Shader> vertexShader;
	std::shared_ptr<Shader> pixelShader;


	// Render Queue:

	std::unique_ptr<RenderQueue> renderQueue;
	std::shared_ptr<std::vector<MeshObject*>> meshRenderQueue;


	// Constant buffers:
	// The renderer keeps these constant buffers since only one is ever required
	// So it just updates them with data for each object every frame

	std::unique_ptr<ConstantBuffer> cameraBuffer;
	std::unique_ptr<ConstantBuffer> worldMatrixBuffer;

	// ImGui variables
	
	bool isVSyncEnabled = false;

	void SetViewport(const Window& window);
	void CreateDeviceAndSwapChain(const Window& window);
	void CreateRenderTarget();
	void CreateDepthBuffer(const Window& window);
	void CreateInputLayout(const std::string& vShaderByteCode);
	void CreateSampler();
	void CreateStandardRasterizerState();

	/// <summary>
	/// Creates required constant buffers. The renderer needs a cameraBuffer and worldMatrixBuffer.
	/// </summary>
	void CreateRendererConstantBuffers();

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

	void ResizeSwapChain(const Window& window);

	void BindSampler();
	void BindInputLayout();
	void BindRenderTarget();
	void BindViewport();
	void BindRasterizerState(RasterizerState* rastState);

	void BindMaterial(Material* material);

	void BindCameraMatrix();
	void BindWorldMatrix(ID3D11Buffer* buffer);

	/// <summary>
	/// Renders a single MeshObject
	/// </summary>
	/// <param name="meshObject"></param>
	void RenderMeshObject(MeshObject* meshObject);
};