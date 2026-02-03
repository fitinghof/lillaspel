#include "rendering/renderer.h"
#include "gameObjects/objectLoader.h"

Renderer::Renderer() : viewport(), maximumSpotlights(16)
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

void Renderer::CreateRendererConstantBuffers()
{
	CameraObject::CameraMatrixContainer camMatrix = {};
	this->cameraBuffer = std::make_unique<ConstantBuffer>();
	this->cameraBuffer->Init(this->device.Get(), sizeof(CameraObject::CameraMatrixContainer), &camMatrix, D3D11_USAGE_DYNAMIC, D3D11_CPU_ACCESS_WRITE);

	Renderer::WorldMatrixBufferContainer worldMatrix = {};
	this->worldMatrixBuffer = std::make_unique<ConstantBuffer>();
	worldMatrixBuffer->Init(this->device.Get(), sizeof(Renderer::WorldMatrixBufferContainer), &worldMatrix, D3D11_USAGE_DYNAMIC, D3D11_CPU_ACCESS_WRITE);

	SpotlightObject::SpotLightContainer defaultSpotlights[1];
	this->spotlightBuffer = std::make_unique<StructuredBuffer>();
	this->spotlightBuffer->Init(this->device.Get(), sizeof(SpotlightObject::SpotLightContainer), this->maximumSpotlights, defaultSpotlights);

	Renderer::LightCountBufferContainer lightCountContainer = {};
	this->spotlightCountBuffer = std::make_unique<ConstantBuffer>();
	this->spotlightCountBuffer->Init(this->device.Get(), sizeof(Renderer::LightCountBufferContainer), &lightCountContainer, D3D11_USAGE_DYNAMIC, D3D11_CPU_ACCESS_WRITE);
}

void Renderer::CreateRenderQueue()
{
	this->meshRenderQueue = this->meshRenderQueue = std::make_shared<std::vector<MeshObject*>>();
	this->lightRenderQueue = this->lightRenderQueue = std::make_shared<std::vector<SpotlightObject*>>();
	this->renderQueue = std::unique_ptr<RenderQueue>(new RenderQueue(this->meshRenderQueue, this->lightRenderQueue));
}

void Renderer::LoadShaders(std::string& vShaderByteCode)
{
	// This shouldn't be directly hardcoded into the renderer

	this->vertexShader = std::shared_ptr<Shader>(new Shader());
	this->vertexShader->Init(this->device.Get(), ShaderType::VERTEX_SHADER, "VSTest.cso");
	vShaderByteCode = this->vertexShader->GetShaderByteCode();

	this->pixelShaderLit = std::shared_ptr<Shader>(new Shader());
	this->pixelShaderLit->Init(this->device.Get(), ShaderType::PIXEL_SHADER, "psLit.cso");

	this->pixelShaderUnlit = std::shared_ptr<Shader>(new Shader());
	this->pixelShaderUnlit->Init(this->device.Get(), ShaderType::PIXEL_SHADER, "psUnlit.cso");

	this->defaultMat = std::unique_ptr<Material>(new Material);
	this->defaultMat->Init(this->vertexShader, this->pixelShaderLit);

	this->defaultUnlitMat = std::unique_ptr<Material>(new Material);
	this->defaultUnlitMat->Init(this->vertexShader, this->pixelShaderUnlit);

	Material::BasicMaterialStruct defaultMatColor{ {0.3f,0.3f,0.3f,1}, {1,1,1,1}, {1,1,1,1}, 100, 0, {1,1} };

	this->defaultMat->pixelShaderBuffers.push_back(std::make_unique<ConstantBuffer>());
	this->defaultMat->pixelShaderBuffers[0]->Init(this->device.Get(), sizeof(Material::BasicMaterialStruct), &defaultMatColor, D3D11_USAGE_IMMUTABLE, 0);
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

	// Bind standard stuff (most of it probably doesn't need to be set every frame)
	BindSampler();
	BindInputLayout();
	BindRenderTarget();
	BindViewport();
	BindRasterizerState(this->standardRasterizerState.get());

	// Bind frame specific stuff
	BindMaterial(this->defaultMat.get());
	BindCameraMatrix();
	BindLights();

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
	if (rastState == nullptr)
	{
		Logger::Error("RasterizerSate is nullptr");
	}

	this->immediateContext->RSSetState(rastState->GetRasterizerState());
}

void Renderer::BindMaterial(Material* material)
{
	// Bind shaders
	material->vertexShader->BindShader(this->immediateContext.Get());
	material->pixelShader->BindShader(this->immediateContext.Get());

	// Also bind constant buffers
	for (size_t i = 0; i < material->pixelShaderBuffers.size(); i++)
	{
		ID3D11Buffer* buf = material->pixelShaderBuffers[i]->GetBuffer();
		this->immediateContext->PSSetConstantBuffers(i + 1, 1, &buf); // i + 1 because first slot is always occupied
	}

	for (size_t i = 0; i < material->vertexShaderBuffers.size(); i++)
	{
		ID3D11Buffer* buf = material->vertexShaderBuffers[i]->GetBuffer();
		this->immediateContext->VSSetConstantBuffers(i + 2, 1, &buf); // i + 2 because the first two slots are always occupied
	}
}

void Renderer::BindLights()
{
	if (this->lightRenderQueue->size() > this->maximumSpotlights)
	{
		Logger::Log("Just letting you know, there's more lights in the scene than the renderer supports. Increase maximumSpotlights.");
	}

	const size_t lightCount = std::min<size_t>(this->lightRenderQueue->size(), this->maximumSpotlights);

	if (lightCount > 0) {
		//Logger::Log("No light. Please add a light to the scene.");

		// Inefficient, should be fixed
		std::vector<SpotlightObject::SpotLightContainer> spotlights;
		for (size_t i = 0; i < lightCount; i++)
		{
			spotlights.push_back((*this->lightRenderQueue)[i]->data);
		}

		// Updates and binds buffer
		this->spotlightBuffer->UpdateBuffer(this->immediateContext.Get(), spotlights.data());
		ID3D11ShaderResourceView* lightSrv = this->spotlightBuffer->GetSRV();
		this->immediateContext->PSSetShaderResources(1, 1, &lightSrv);
	}

	// Updates and binds light count constant buffer
	Renderer::LightCountBufferContainer lightCountContainer = { lightCount, 1, 1, 1 };
	this->spotlightCountBuffer->UpdateBuffer(this->immediateContext.Get(), &lightCountContainer);
	ID3D11Buffer* buf = this->spotlightCountBuffer->GetBuffer();
	this->immediateContext->PSSetConstantBuffers(0, 1, &buf);
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


	for (auto subMesh : meshObject->GetMesh()->GetSubMeshes())
	{
		ID3D11ShaderResourceView* textureSrv = subMesh.GetTexture().GetSrv();
		this->immediateContext->PSSetShaderResources(0, 1, &textureSrv);

		// Draw to screen
		this->immediateContext->DrawIndexed(subMesh.GetNrOfIndices(), subMesh.GetStartIndex(), 0);
	}
}
