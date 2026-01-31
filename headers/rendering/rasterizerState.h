#pragma once
#include <d3d11.h>
#include <wrl/client.h>
#include <iostream>
#include <format>
#include "utilities/logger.h"

class RasterizerState {
public:
	RasterizerState() = default;
	~RasterizerState() = default;

	void Init(ID3D11Device* device, const D3D11_RASTERIZER_DESC* desc);

	ID3D11RasterizerState* GetRasterizerState();
private:
	Microsoft::WRL::ComPtr<ID3D11RasterizerState> rasterizerState;;
};