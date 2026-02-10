#include "rendering/renderer.h"
#include "gameObjects/objectLoader.h"

Renderer::Renderer()
	: viewport(), currentPixelShader(nullptr), currentVertexShader(nullptr), currentRasterizerState(nullptr),
	  maximumSpotlights(16) {
}

void Renderer::Init(const Window& window)
{
	SetViewport(window);

	CreateDeviceAndSwapChain(window);
	CreateRenderTarget();
	CreateDepthBuffer(window);

	CreateRenderQueue();
}

void Renderer::SetAllDefaults()
{
	CreateSampler();

	CreateInputLayout(AssetManager::GetInstance().GetShaderPtr("VSStandard")->GetShaderByteCode());

	CreateRasterizerStates();

	CreateRendererConstantBuffers();

	LoadShaders();
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

void Renderer::CreateRasterizerStates()
{
	D3D11_RASTERIZER_DESC rastDesc;
	ZeroMemory(&rastDesc, sizeof(rastDesc));
	rastDesc.CullMode = D3D11_CULL_BACK;
	rastDesc.DepthClipEnable = TRUE;
	rastDesc.FillMode = D3D11_FILL_SOLID;
	this->standardRasterizerState = std::make_unique<RasterizerState>();
	this->standardRasterizerState->Init(this->device.Get(), &rastDesc);

	D3D11_RASTERIZER_DESC wireframeRastDesc;
	ZeroMemory(&wireframeRastDesc, sizeof(wireframeRastDesc));
	wireframeRastDesc.CullMode = D3D11_CULL_NONE;
	wireframeRastDesc.DepthClipEnable = TRUE;
	wireframeRastDesc.FillMode = D3D11_FILL_WIREFRAME; // Wireframe
	this->wireframeRasterizerState = std::make_unique<RasterizerState>();
	this->wireframeRasterizerState->Init(this->device.Get(), &wireframeRastDesc);
}

void Renderer::CreateRendererConstantBuffers()
{
	CameraObject::CameraMatrixContainer camMatrix = {};
	this->cameraBuffer = std::make_unique<ConstantBuffer>();
	this->cameraBuffer->Init(this->device.Get(), sizeof(CameraObject::CameraMatrixContainer), &camMatrix, D3D11_USAGE_DYNAMIC, D3D11_CPU_ACCESS_WRITE);

	Renderer::WorldMatrixBufferContainer worldMatrix = {};
	this->worldMatrixBuffer = std::make_unique<ConstantBuffer>();
	this->worldMatrixBuffer->Init(this->device.Get(), sizeof(Renderer::WorldMatrixBufferContainer), &worldMatrix, D3D11_USAGE_DYNAMIC, D3D11_CPU_ACCESS_WRITE);

	SpotlightObject::SpotLightContainer defaultSpotlights[1];
	this->spotlightBuffer = std::make_unique<StructuredBuffer>();
	this->spotlightBuffer->Init(this->device.Get(), sizeof(SpotlightObject::SpotLightContainer), this->maximumSpotlights, defaultSpotlights);

	Renderer::LightCountBufferContainer lightCountContainer = {};
	this->spotlightCountBuffer = std::make_unique<ConstantBuffer>();
	this->spotlightCountBuffer->Init(this->device.Get(), sizeof(Renderer::LightCountBufferContainer), &lightCountContainer, D3D11_USAGE_DYNAMIC, D3D11_CPU_ACCESS_WRITE);
}

void Renderer::CreateRenderQueue()
{
	this->meshRenderQueue = std::make_shared<std::vector<std::weak_ptr<MeshObject>>>();
	this->lightRenderQueue = std::make_shared<std::vector<std::weak_ptr<SpotlightObject>>>();
	this->renderQueue = std::unique_ptr<RenderQueue>(new RenderQueue(this->meshRenderQueue, this->lightRenderQueue));
}

void Renderer::LoadShaders()
{
	this->vertexShader = AssetManager::GetInstance().GetShaderPtr("VSStandard");
	this->pixelShaderLit = AssetManager::GetInstance().GetShaderPtr("PSLit");
	this->pixelShaderUnlit = AssetManager::GetInstance().GetShaderPtr("PSUnlit");

	this->defaultMat = AssetManager::GetInstance().GetMaterialWeakPtr("defaultLitMaterial");
	this->defaultUnlitMat = AssetManager::GetInstance().GetMaterialWeakPtr("defaultUnlitMaterial");
}

void Renderer::Render()
{
	RenderPass();
}

void Renderer::Present()
{
	this->swapChain->Present(this->isVSyncEnabled ? 1 : 0, 0);
}

void Renderer::Resize(const Window& window)
{
	this->ResizeSwapChain(window);

	BindRenderTarget();
	BindViewport();
}

void Renderer::ToggleVSync(bool enable)
{
	this->isVSyncEnabled = enable;
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
	// THIS IS THE ISSUE WITH SCALING PROBABLY
	// Bind basic stuff (it probably doesn't need to be set every frame)
	if (!this->hasBoundStatic) {
		BindSampler();
		BindInputLayout();
		BindRenderTarget();
		BindViewport();

		this->hasBoundStatic = true;
	}

	// Clear last frame
	ClearRenderTargetViewAndDepthStencilView();

	// Bind rasterizerState
	if (!this->renderAllWireframe) 
	{
		BindRasterizerState(this->standardRasterizerState.get());
	} 
	else
	{
		BindRasterizerState(this->wireframeRasterizerState.get());
		BindMaterial(this->defaultUnlitMat.lock().get());
	}

	// Bind frame specific stuff
	BindCameraMatrix();
	BindLights();

	// Bind meshes
	for (size_t i = 0; i < this->meshRenderQueue->size(); i++)
	{
		if ((*this->meshRenderQueue)[i].expired())
		{
			// This should get rid of empty objects
			Logger::Log("The renderer deleted a meshObject");
			this->meshRenderQueue->erase(this->meshRenderQueue->begin() + i);
			i--;
			continue;
		}

		RenderMeshObject((*this->meshRenderQueue)[i].lock().get());
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
	if (rastState == nullptr)
	{
		Logger::Error("RasterizerSate is nullptr");
	}

	this->immediateContext->RSSetState(rastState->GetRasterizerState());

	this->currentRasterizerState = rastState;
}

void Renderer::BindMaterial(BaseMaterial* material)
{
	RenderData renderData = material->GetRenderData(this->immediateContext.Get());

	if (material->wireframe) 
	{
		if (this->currentRasterizerState != this->wireframeRasterizerState.get()) {
			BindRasterizerState(this->wireframeRasterizerState.get());
		}
	} 
	else 
	{
		if (this->currentRasterizerState == this->wireframeRasterizerState.get()) 
		{
			BindRasterizerState(this->standardRasterizerState.get());
		}
	}

	// Bind shaders
	// Checks to avoid making unnecessary GPU calls
	// Since shaders will almost always be the same
	if (renderData.vertexShader)
	{
		if (this->currentVertexShader != renderData.vertexShader.get())
		{
			renderData.vertexShader->BindShader(this->immediateContext.Get());
			this->currentVertexShader = renderData.vertexShader.get();
		}
	}
	else
	{
		if (this->currentVertexShader != this->vertexShader.get())
		{
			this->vertexShader->BindShader(this->immediateContext.Get());
			this->currentVertexShader = this->vertexShader.get();
		}
	}

	if (renderData.pixelShader)
	{
		if (this->currentPixelShader != renderData.pixelShader.get())
		{
			renderData.pixelShader->BindShader(this->immediateContext.Get());
			this->currentPixelShader = renderData.pixelShader.get();
		}
	}
	else
	{
		if (this->currentPixelShader != this->pixelShaderLit.get())
		{
			this->pixelShaderLit->BindShader(this->immediateContext.Get());
			this->currentPixelShader = this->pixelShaderLit.get();
		}
	}

	// FIX
	this->immediateContext->PSSetShaderResources(1, 1/*renderData.textures.size()*/, renderData.textures.data());

	// Also bind constant buffers
	for (size_t i = 0; i < renderData.pixelBuffers.size(); i++)
	{
		ID3D11Buffer* buf = renderData.pixelBuffers[i]->GetBuffer();
		this->immediateContext->PSSetConstantBuffers(i + 1, 1, &buf); // i + 1 because first slot is always occupied
	}

	for (size_t i = 0; i < renderData.pixelBuffers.size(); i++)
	{
		ID3D11Buffer* buf = renderData.pixelBuffers[i]->GetBuffer();
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

		// Inefficient, should be fixed
		std::vector<SpotlightObject::SpotLightContainer> spotlights;
		for (size_t i = 0; i < lightCount; i++)
		{
			if ((*this->lightRenderQueue)[i].expired()) 
			{
				// This should remove deleted lights
				Logger::Log("The renderer deleted a light");
				this->lightRenderQueue->erase(this->lightRenderQueue->begin() + i);
				i--;
				continue;
			}

			spotlights.push_back((*this->lightRenderQueue)[i].lock()->data);
		}

		// Updates and binds buffer
		this->spotlightBuffer->UpdateBuffer(this->immediateContext.Get(), spotlights.data());
		ID3D11ShaderResourceView* lightSrv = this->spotlightBuffer->GetSRV();
		this->immediateContext->PSSetShaderResources(0, 1, &lightSrv);
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
	MeshObjData data = meshObject->GetMesh();
	std::weak_ptr<Mesh> weak_mesh = data.GetMesh();
	if (weak_mesh.expired()) {
		Logger::Error("Trying to render with expired mesh");
		return;
	}

	std::shared_ptr<Mesh> mesh = weak_mesh.lock();

	VertexBuffer vBuf = mesh->GetVertexBuffer();

	UINT stride = vBuf.GetVertexSize();
	UINT offset = 0;
	ID3D11Buffer* vBuff = vBuf.GetBuffer();
	this->immediateContext->IASetVertexBuffers(0, 1, &vBuff, &stride, &offset);
	this->immediateContext->IASetIndexBuffer(mesh->GetIndexBuffer().GetBuffer(), DXGI_FORMAT_R32_UINT, 0);



	// Bind worldmatrix
	DirectX::XMFLOAT4X4 worldMatrix;
	DirectX::XMStoreFloat4x4(&worldMatrix, meshObject->GetGlobalWorldMatrix(false));
	DirectX::XMFLOAT4X4 worldMatrixInverseTransposed;
	DirectX::XMStoreFloat4x4(&worldMatrixInverseTransposed, meshObject->GetGlobalWorldMatrix(true));

	Renderer::WorldMatrixBufferContainer worldMatrixBufferContainer = { worldMatrix, worldMatrixInverseTransposed };

	this->worldMatrixBuffer->UpdateBuffer(this->immediateContext.Get(), &worldMatrixBufferContainer);
	BindWorldMatrix(this->worldMatrixBuffer->GetBuffer());


	// Draw submeshes
	size_t index = 0;
	for (auto& subMesh : mesh->GetSubMeshes())
	{
		std::weak_ptr<BaseMaterial> weak_material = data.GetMaterial(index);

		if (weak_material.expired())
		{
			Logger::Error("Trying to render expired material, trying to continue...");
		}
		else
		{
			if (!this->renderAllWireframe)
			{
				BindMaterial(weak_material.lock().get());
			}

			// Draw to screen
			this->immediateContext->DrawIndexed(subMesh.GetNrOfIndices(), subMesh.GetStartIndex(), 0);
		}

		index++;
	}
}
