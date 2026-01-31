#include "rendering/renderer.h"

Renderer::Renderer() : viewport(), mesh()
{
}

void Renderer::Init(const Window& window)
{
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

	ObjectLoader l;
	//this->mesh = l.LoadGltf("", this->device.Get());

	CameraObject::CameraMatrixContainer camMatrix = {};
	this->cameraBuffer = std::make_unique<ConstantBuffer>();
	this->cameraBuffer->Init(this->device.Get(), sizeof(CameraObject::CameraMatrixContainer), &camMatrix, D3D11_USAGE_DYNAMIC, D3D11_CPU_ACCESS_WRITE);

}

void Renderer::SetViewport(const Window& window)
{
	this->viewport.Width = static_cast<FLOAT>(window.GetWidth());
	this->viewport.Height = static_cast<FLOAT>(window.GetHeight());
	this->viewport.MinDepth = 0.0f;
	this->viewport.MaxDepth = 1.0f;
	this->viewport.TopLeftX = 0;
	this->viewport.TopLeftY = 0;
}

void Renderer::CreateDeviceAndSwapChain(const Window& window)
{
	DXGI_SWAP_CHAIN_DESC swapChainDesc = {};
	swapChainDesc.BufferCount = 2;
	swapChainDesc.BufferDesc.Width = 0;
	swapChainDesc.BufferDesc.Height = 0;
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
	this->depthBuffer = std::unique_ptr<DepthBuffer>(new DepthBuffer());
	this->depthBuffer->Init(this->device.Get(), window.GetWidth(), window.GetHeight());
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

void Renderer::CreateRenderQueue()
{
	this->meshRenderQueue = std::shared_ptr<std::vector<int>>();
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

	BindSampler();
	BindInputLayout();
	BindRenderTarget();
	BindViewport();

	BindMaterial(this->tempMat.get());

	BindRasterizerState(this->standardRasterizerState.get());

	BindCameraMatrix();



	// Temporary logic to create a quad
	// Will be replaced when we can use a mesh class instead

	Vertex vertexData[] = {
		{-1, -1, 0,		0.0f, 0.0f, -1.0f,	1.0f, 1.0f},
		{-1,  1, 0,		0.0f, 0.0f, -1.0f,	1.0f, 1.0f},
		{ 1, -1, 0,		0.0f, 0.0f, -1.0f,	1.0f, 1.0f},
		{ 1,  1, 0,		0.0f, 0.0f, -1.0f,	1.0f, 1.0f}
	};

	std::unique_ptr<VertexBuffer> tempVBuffer = std::unique_ptr<VertexBuffer>(new VertexBuffer());
	tempVBuffer->Init(this->device.Get(), sizeof(Vertex), 4, vertexData);

	UINT stride = tempVBuffer->GetVertexSize();
	UINT offset = 0;
	ID3D11Buffer* vBuff = tempVBuffer->GetBuffer();

	this->immediateContext->IASetVertexBuffers(0, 1, &vBuff, &stride, &offset);

	uint32_t indices[] = {
		0,1,2,1,3,2
	};

	std::unique_ptr<IndexBuffer> tempIBuffer = std::unique_ptr<IndexBuffer>(new IndexBuffer());
	tempIBuffer->Init(this->device.Get(), 6, indices);

	this->immediateContext->IASetIndexBuffer(tempIBuffer->GetBuffer(), DXGI_FORMAT_R32_UINT, 0);

	// Testing for actual meshes:

	//UINT stride = this->mesh.GetVertexBuffer().GetVertexSize();
	//UINT offset = 0;
	//ID3D11Buffer* vBuff = this->mesh.GetVertexBuffer().GetBuffer();
	////Logger::Log(this->mesh.GetVertexBuffer().GetNrOfVertices());
	//this->immediateContext->IASetVertexBuffers(0, 1, &vBuff, &stride, &offset);
	//this->immediateContext->IASetIndexBuffer(this->mesh.GetIndexBuffer().GetBuffer(), DXGI_FORMAT_R32_UINT, 0);

	float meshPos[3] = { 0.0f, 0.0f, 10.0f };
	static float rot = 0;
	float meshRot[3] = { 0.0f, rot += 0.01f, 0.0f}; // I know this is framerate-dependent. It's a temporary test, ok?
	float meshScale[3] = { 1.0f, 1.0f, 1.0f };

	MatrixContainer* worldMatrix = nullptr;
	ConstantBufferWorldMatrix(worldMatrix, meshPos, meshRot, meshScale);

	MatrixContainer* worldMatrixInverseTransposed = nullptr;
	ConstantBufferWorldMatrix(worldMatrixInverseTransposed, meshPos, meshRot, meshScale, true);

	WorldMatrixBufferContainer worldMatrixBufferContainer = { *worldMatrix, *worldMatrixInverseTransposed };

	std::unique_ptr<ConstantBuffer> worldMatrixBuffer = std::make_unique<ConstantBuffer>();
	worldMatrixBuffer->Init(this->device.Get(), sizeof(worldMatrixBufferContainer), &worldMatrixBufferContainer, D3D11_USAGE_DYNAMIC, D3D11_CPU_ACCESS_WRITE);

	BindWorldMatrix(worldMatrixBuffer->GetBuffer());

	delete worldMatrix;
	delete worldMatrixInverseTransposed;





	// Draw the quad to screen
	this->immediateContext->DrawIndexed(tempIBuffer->GetNrOfIndices(), 0, 0);
}

void Renderer::ClearRenderTargetViewAndDepthStencilView()
{
	// Clear previous frame
	float clearColor[4] = { 0,0,0.1,0 };
	this->immediateContext->ClearRenderTargetView(this->renderTarget->GetRenderTargetView(), clearColor);
	this->immediateContext->ClearDepthStencilView(this->depthBuffer->GetDepthStencilView(0), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1, 0);
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
