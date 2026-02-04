#pragma once
#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <d3d11.h>
#include <wrl/client.h>
#include <vector>
#include <format>

class DepthBuffer {
private:
	Microsoft::WRL::ComPtr<ID3D11Texture2D> texture;
	std::vector<Microsoft::WRL::ComPtr<ID3D11DepthStencilView>> depthStencilViews;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> shaderResourceView;
public:
	DepthBuffer() = default;
	~DepthBuffer() = default;

	void Init(ID3D11Device* device, UINT width, UINT height);

	ID3D11DepthStencilView* GetDepthStencilView(UINT arrayIndex) const;
	ID3D11ShaderResourceView* GetShaderResourceView() const;
};