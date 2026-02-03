#include "rendering/renderer.h"
#include "gameObjects/objectLoader.h"

Renderer::Renderer() : viewport()
{
}

void Renderer::Init(const Window& window)
{
	ObjectLoader objectLoader;
	SetViewport(window);

	CreateDeviceAndSwapChain(window);
	CreateRenderTarget();
	CreateDepthBuffer(window);
	CreateSampler();

	std::string vShaderByteCode;
	LoadShaders(vShaderByteCode);
	CreateInputLayout(vShaderByteCode);

	CreateStandardRasterizerState();

	CreateRenderQueue();

	CreateRendererConstantBuffers();
}

void Renderer::SetViewport(const Window& window)
{
	RECT rc{};
	GetClientRect(window.GetHWND(), &rc);
	UINT clientWidth = static_cast<UINT>(rc.right - rc.left);
	UINT clientHeight = static_cast<UINT>(rc.bottom - rc.top);
	if (clientWidth == 0 || clientHeight == 0) {
		clientWidth = window.GetWidth();
		clientHeight = window.GetHeight();
	}
	if (clientWidth == 0 || clientHeight == 0)
	{
		clientWidth = 1;
		clientHeight = 1;
	}

	this->viewport.Width = static_cast<FLOAT>(clientWidth);
	this->viewport.Height = static_cast<FLOAT>(clientHeight);
	this->viewport.MinDepth = 0.0f;
	this->viewport.MaxDepth = 1.0f;
	this->viewport.TopLeftX = 0;
	this->viewport.TopLeftY = 0;
}

void Renderer::CreateDeviceAndSwapChain(const Window& window)
{
	RECT rc{};
	GetClientRect(window.GetHWND(), &rc);
	UINT clientWidth = static_cast<UINT>(rc.right - rc.left);
	UINT clientHeight = static_cast<UINT>(rc.bottom - rc.top);
	if (clientWidth == 0 || clientHeight == 0) {
		clientWidth = window.GetWidth();
		clientHeight = window.GetHeight();
	}

	DXGI_SWAP_CHAIN_DESC swapChainDesc = {};
	swapChainDesc.BufferCount = 2;
	swapChainDesc.BufferDesc.Width = clientWidth;
	swapChainDesc.BufferDesc.Height = clientHeight;
	swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	swapChainDesc.BufferDesc.RefreshRate.Numerator = 0;
	swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
	swapChainDesc.SampleDesc.Count = 1;
	swapChainDesc.SampleDesc.Quality = 0;
	swapChainDesc.BufferUsage = DXGI_USAGE_UNORDERED_ACCESS | DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.OutputWindow = window.GetHWND();
	swapChainDesc.Windowed = TRUE;
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	swapChainDesc.Flags = 0;

	HRESULT hr = D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, D3D11_CREATE_DEVICE_DEBUG, nullptr, 0,
		D3D11_SDK_VERSION, &swapChainDesc, this->swapChain.GetAddressOf(),
		this->device.GetAddressOf(), nullptr, this->immediateContext.GetAddressOf());

	if (FAILED(hr))
	{
		throw std::runtime_error("Failed to create device and swapchain, Error: " + hr);
	}
}

void Renderer::CreateRenderTarget()
{
	this->renderTarget = std::unique_ptr<RenderTarget>(new RenderTarget());
	this->renderTarget->Init(this->device.Get(), this->swapChain.Get());
}

void Renderer::CreateDepthBuffer(const Window& window)
{
	RECT rc{};
	GetClientRect(window.GetHWND(), &rc);
	UINT clientWidth = static_cast<UINT>(rc.right - rc.left);
	UINT clientHeight = static_cast<UINT>(rc.bottom - rc.top);

	if (clientWidth == 0 || clientHeight == 0) {
		clientWidth = window.GetWidth();
		clientHeight = window.GetHeight();
	}

	if (clientWidth == 0 || clientHeight == 0) {
		DXGI_SWAP_CHAIN_DESC desc{};
		if (SUCCEEDED(this->swapChain->GetDesc(&desc))) {
			clientWidth = desc.BufferDesc.Width;
			clientHeight = desc.BufferDesc.Height;
		}
	}

	if (clientWidth == 0 || clientHeight == 0) {
		clientWidth = 1;
		clientHeight = 1;
	}

	this->depthBuffer = std::unique_ptr<DepthBuffer>(new DepthBuffer());
	this->depthBuffer->Init(this->device.Get(), clientWidth, clientHeight);
}

void Renderer::CreateInputLayout(const std::string& vShaderByteCode)
{
	this->inputLayout = std::unique_ptr<InputLayout>(new InputLayout());
	this->inputLayout->AddInputElement("POSITION", DXGI_FORMAT_R32G32B32_FLOAT);
	this->inputLayout->AddInputElement("NORMAL", DXGI_FORMAT_R32G32B32_FLOAT);
	this->inputLayout->AddInputElement("UV", DXGI_FORMAT_R32G32_FLOAT);
	this->inputLayout->FinalizeInputLayout(this->device.Get(), vShaderByteCode.c_str(), vShaderByteCode.length());
}

void Renderer::CreateSampler()
{
	this->sampler = std::unique_ptr<Sampler>(new Sampler());
	this->sampler->Init(this->device.Get(), D3D11_TEXTURE_ADDRESS_WRAP);
}

void Renderer::CreateStandardRasterizerState()
{
	D3D11_RASTERIZER_DESC rastDesc;
	ZeroMemory(&rastDesc, sizeof(rastDesc));
	rastDesc.CullMode = D3D11_CULL_NONE;
	rastDesc.DepthClipEnable = TRUE;
	rastDesc.FillMode = D3D11_FILL_SOLID;
	this->standardRasterizerState = std::make_unique<RasterizerState>();
	this->standardRasterizerState->Init(this->device.Get(), &rastDesc);
}

void Renderer::CreateRendererConstantBuffers()
{
	CameraObject::CameraMatrixContainer camMatrix = {};
	this->cameraBuffer = std::make_unique<ConstantBuffer>();
	this->cameraBuffer->Init(this->device.Get(), sizeof(CameraObject::CameraMatrixContainer), &camMatrix, D3D11_USAGE_DYNAMIC, D3D11_CPU_ACCESS_WRITE);

	Renderer::WorldMatrixBufferContainer worldMatrix = {};
	this->worldMatrixBuffer = std::make_unique<ConstantBuffer>();
	worldMatrixBuffer->Init(this->device.Get(), sizeof(Renderer::WorldMatrixBufferContainer), &worldMatrix, D3D11_USAGE_DYNAMIC, D3D11_CPU_ACCESS_WRITE);
}

void Renderer::CreateRenderQueue()
{
	this->meshRenderQueue = this->meshRenderQueue = std::make_shared<std::vector<MeshObject*>>();
	this->renderQueue = std::unique_ptr<RenderQueue>(new RenderQueue(this->meshRenderQueue));
}

void Renderer::LoadShaders(std::string& vShaderByteCode)
{
	// This shouldn't be directly hardcoded into the renderer

	this->vertexShader = std::shared_ptr<Shader>(new Shader());
	this->vertexShader->Init(this->device.Get(), ShaderType::VERTEX_SHADER, "VSTest.cso");
	vShaderByteCode = this->vertexShader->GetShaderByteCode();

	this->pixelShader = std::shared_ptr<Shader>(new Shader());
	this->pixelShader->Init(this->device.Get(), ShaderType::PIXEL_SHADER, "PSTest.cso");

	this->tempMat = std::unique_ptr<Material>(new Material);
	this->tempMat->Init(this->vertexShader, this->pixelShader);
}

void Renderer::Render()
{
	RenderPass();
}

void Renderer::Present()
{
	this->swapChain->Present(0, 0);
}

void Renderer::Resize(const Window& window)
{
	this->ResizeSwapChain(window);
}

ID3D11Device* Renderer::GetDevice() const
{
	return this->device.Get();
}

ID3D11DeviceContext* Renderer::GetContext() const
{
	return this->immediateContext.Get();
}

IDXGISwapChain* Renderer::GetSwapChain() const
{
	return this->swapChain.Get();
}

void Renderer::RenderPass()
{
	ClearRenderTargetViewAndDepthStencilView();

	// Bind standard stuff (most of it probably doesn't need to be set every frame)
	BindSampler();
	BindInputLayout();
	BindRenderTarget();
	BindViewport();
	BindRasterizerState(this->standardRasterizerState.get());

	// Bind frame specific stuff
	BindMaterial(this->tempMat.get());
	BindCameraMatrix();

	// Bind meshes
	for (size_t i = 0; i < meshRenderQueue->size(); i++)
	{
		if ((*meshRenderQueue)[i] == nullptr)
		{
			throw std::runtime_error("nullptr in render queue");
		}

		RenderMeshObject((*meshRenderQueue)[i]);
	}
}

void Renderer::ClearRenderTargetViewAndDepthStencilView()
{
	// Clear previous frame
	float clearColor[4] = { 0,0,0.1,0 };
	this->immediateContext->ClearRenderTargetView(this->renderTarget->GetRenderTargetView(), clearColor);
	this->immediateContext->ClearDepthStencilView(this->depthBuffer->GetDepthStencilView(0), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1, 0);
}

void Renderer::ResizeSwapChain(const Window& window)
{
	if (window.GetWidth() == 0 || window.GetHeight() == 0) {
		return;
	}

	// Unbind any views using the swapchain
	if (this->immediateContext) {
		this->immediateContext->OMSetRenderTargets(0, nullptr, nullptr);
	}

	// Release old views
	this->renderTarget.reset();
	this->depthBuffer.reset();

	// Resize swapchain
	HRESULT hr = this->swapChain->ResizeBuffers(0, window.GetWidth(), window.GetHeight(), DXGI_FORMAT_UNKNOWN, 0);
	if (FAILED(hr))
	{
		throw std::runtime_error("Failed to resize swapchain buffers, Error: " + hr);
	}

	// Recreate views
	CreateRenderTarget();
	CreateDepthBuffer(window);

	// Update viewport
	SetViewport(window);
}

void Renderer::BindSampler()
{
	// Sampler
	ID3D11SamplerState* s = this->sampler->GetSamplerState();
	immediateContext->PSSetSamplers(0, 1, &s);
}

void Renderer::BindInputLayout()
{
	// Set up inputs
	this->immediateContext->IASetInputLayout(this->inputLayout->GetInputLayout());
	this->immediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}

void Renderer::BindRenderTarget()
{
	// Render target
	ID3D11RenderTargetView* rtv = this->renderTarget->GetRenderTargetView();
	this->immediateContext->OMSetRenderTargets(1, &rtv, this->depthBuffer->GetDepthStencilView(0));
}

void Renderer::BindViewport()
{
	this->immediateContext->RSSetViewports(1, &this->viewport);
}

void Renderer::BindRasterizerState(RasterizerState* rastState)
{
	if (rastState == nullptr) {
		Logger::Error("RasterizerSate is nullptr");
	}

	this->immediateContext->RSSetState(rastState->GetRasterizerState());
}

void Renderer::BindMaterial(Material* material)
{
	material->vertexShader->BindShader(this->immediateContext.Get());
	material->pixelShader->BindShader(this->immediateContext.Get());

	// Also bind constant buffers
}

void Renderer::BindCameraMatrix()
{
	this->cameraBuffer->UpdateBuffer(this->immediateContext.Get(), &CameraObject::GetMainCamera().GetCameraMatrix());

	ID3D11Buffer* buffer = this->cameraBuffer->GetBuffer();
	this->immediateContext->VSSetConstantBuffers(0, 1, &buffer);
}

void Renderer::BindWorldMatrix(ID3D11Buffer* buffer)
{
	this->immediateContext->VSSetConstantBuffers(1, 1, &buffer);
}

void Renderer::RenderMeshObject(MeshObject* meshObject)
{
	// Bind mesh
	VertexBuffer vBuf = meshObject->GetMesh()->GetVertexBuffer();

	UINT stride = vBuf.GetVertexSize();
	UINT offset = 0;
	ID3D11Buffer* vBuff = vBuf.GetBuffer();
	this->immediateContext->IASetVertexBuffers(0, 1, &vBuff, &stride, &offset);
	this->immediateContext->IASetIndexBuffer(meshObject->GetMesh()->GetIndexBuffer().GetBuffer(), DXGI_FORMAT_R32_UINT, 0);



	// Bind worldmatrix
	DirectX::XMFLOAT4X4 worldMatrix = meshObject->transform.GetWorldMatrix(false);
	DirectX::XMFLOAT4X4 worldMatrixInverseTransposed = meshObject->transform.GetWorldMatrix(true);

	Renderer::WorldMatrixBufferContainer worldMatrixBufferContainer = { worldMatrix, worldMatrixInverseTransposed };

	this->worldMatrixBuffer->UpdateBuffer(this->immediateContext.Get(), &worldMatrixBufferContainer);
	BindWorldMatrix(this->worldMatrixBuffer->GetBuffer());



	// Draw to screen
	this->immediateContext->DrawIndexed(meshObject->GetMesh()->GetIndexBuffer().GetNrOfIndices(), 0, 0);
}