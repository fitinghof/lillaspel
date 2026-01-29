#include "rendering/renderer.h"
#include "imgui.h"
#include "imgui_impl_dx11.h"

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
	this->inputLayout->AddInputElement("TEXCOORD", DXGI_FORMAT_R32G32_FLOAT);
	this->inputLayout->FinalizeInputLayout(this->device.Get(), vShaderByteCode.c_str(), vShaderByteCode.length());
}

void Renderer::CreateSampler()
{
	this->sampler = std::unique_ptr<Sampler>(new Sampler());
	this->sampler->Init(this->device.Get(), D3D11_TEXTURE_ADDRESS_WRAP);
}

void Renderer::LoadShaders(std::string& vShaderByteCode)
{
	this->vertexShader = std::unique_ptr<Shader>(new Shader());
	this->vertexShader->Init(this->device.Get(), ShaderType::VERTEX_SHADER, "VSTest.cso");
	vShaderByteCode = this->vertexShader->GetShaderByteCode();

	this->pixelShader = std::unique_ptr<Shader>(new Shader());
	this->pixelShader->Init(this->device.Get(), ShaderType::PIXEL_SHADER, "PSTest.cso");
}

void Renderer::Render()
{
	RenderPass();
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

void Renderer::RenderPass()
{
	// Clear previous frame
	float clearColor[4] = { 0,0,0.1,0 };
	this->immediateContext->ClearRenderTargetView(this->renderTarget->GetRenderTargetView(), clearColor);
	this->immediateContext->ClearDepthStencilView(this->depthBuffer->GetDepthStencilView(0), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1, 0);

	// Sampler
	ID3D11SamplerState* s = this->sampler->GetSamplerState();
	immediateContext->PSSetSamplers(0, 1, &s);

	// Shaders
	this->vertexShader->BindShader(this->immediateContext.Get());
	this->immediateContext->RSSetViewports(1, &this->viewport);
	this->pixelShader->BindShader(this->immediateContext.Get());

	// Set up inputs
	this->immediateContext->IASetInputLayout(this->inputLayout->GetInputLayout());
	this->immediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// Render target
	ID3D11RenderTargetView* rtv = this->renderTarget->GetRenderTargetView();
	this->immediateContext->OMSetRenderTargets(1, &rtv, this->depthBuffer->GetDepthStencilView(0));

	/*MatrixContainer* matData = nullptr;
	XMMATRIX_ViewMatrix(cameraPos, lookPos, up)* XMMATRIX_ProjMatrix_Perspective(fov, aspectRatio);
	CameraBufferContainer data = { *matData, 0, 0, 0 };*/


	// Vertices - this is very temporary
	Vertex vertexData[] = {
		// Triangle 1
		{-0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f, 1.0f, 1.0f},
		{-0.5f,  0.5f, 0.0f,  0.0f, 1.0f, 0.0f, 1.0f, 1.0f},
		{ 0.5f, -0.5f, 0.0f,  0.0f, 0.0f, 1.0f, 1.0f, 1.0f}
	};

	VertexBuffer* tempVBuffer = new VertexBuffer();
	tempVBuffer->Init(this->device.Get(), sizeof(Vertex), 3, vertexData);

	UINT stride = tempVBuffer->GetVertexSize();
	UINT offset = 0;
	ID3D11Buffer* vBuff = tempVBuffer->GetBuffer();

	this->immediateContext->IASetVertexBuffers(0, 1, &vBuff, &stride, &offset);

	this->immediateContext->Draw(3, 0);
	//this->immediateContext->DrawIndexed(mesh->GetIndexBuffer()->GetNrOfIndices(), 0, 0);
}
