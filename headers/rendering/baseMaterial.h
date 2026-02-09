#pragma once

#define NOMINMAX
#define WIN32_LEAN_AND_MEAN

#include "rendering/constantBuffer.h"
#include "rendering/shader.h"
#include <d3d11.h>
#include <memory>
#include <vector>

struct RenderData {
	std::shared_ptr<Shader> pixelShader;
	std::shared_ptr<Shader> vertexShader;
	std::vector<ConstantBuffer> pixelBuffers;
	std::vector<ConstantBuffer> vertexBuffers;
	std::vector<ID3D11ShaderResourceView*> textures;
};

class BaseMaterial {
public:
	struct BasicMaterialStruct {
		float ambient[4];
		float diffuse[4];
		float specular[4];
		float shininess;
		float padding[3];
	};

	virtual RenderData GetRenderData() = 0;

	std::string identifier;
};