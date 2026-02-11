#pragma once

#include <d3d11.h>

#include "rendering/renderTarget.h"
#include "rendering/i

class TextureCube {
private:
	Microsoft::WRL::ComPtr<ID3D11Texture2D> textures = nullptr;
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> rtvs[6] {nullptr};
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> shaderResourceView;

	void CreateTextures(ID3D11Device* device, UINT width, UINT height);
	void CreateTextureRTVs(ID3D11Device* device);
	void CreateTextureSRV(ID3D11Device* device);

	Vector2 resolution;

public:
	TextureCube() = default;
	~TextureCube() = default;

	void Init(ID3D11Device* device, UINT width, UINT height);

	ID3D11RenderTargetView* GetRTV(size_t index);
	ID3D11ShaderResourceView* GetSRV() const;

	//Vector2 GetResolution() const;
};