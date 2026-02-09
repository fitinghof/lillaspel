#pragma once

#define NOMINMAX
#define WIN32_LEAN_AND_MEAN

#include "rendering/shader.h"
#include <d3d11.h>
#include <memory>
#include "rendering/constantBuffer.h"

struct RenderData {
	std::shared_ptr<Shader> pixelShader;
	std::shared_ptr<Shader> vertexShader;
	std::vector<ConstantBuffer*> pixelBuffers;
	std::vector<ConstantBuffer*> vertexBuffers;
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

	BaseMaterial(ID3D11Device* device) {}
	virtual RenderData GetRenderData(ID3D11DeviceContext* context) = 0;

	std::string identifier;

	bool wireframe = false;
};