#pragma once

#define NOMINMAX
#define WIN32_LEAN_AND_MEAN

#include "rendering/baseMaterial.h"

#include <memory>
#include "rendering/texture.h"

#include "rendering/shader.h"

class UnlitMaterial : public BaseMaterial {
public:
	std::weak_ptr<Shader> unlitShader;
	float color[4];
	std::shared_ptr<Texture> diffuseTexture;

	std::unique_ptr<ConstantBuffer> materialInfo;

	UnlitMaterial(ID3D11Device* device) : BaseMaterial(device)
	{
		for (size_t i = 0; i < 4; i++) {
			this->color[i] = 1.0;
		}

		BaseMaterial::BasicMaterialStruct data = {};
		this->materialInfo = std::make_unique<ConstantBuffer>();
		this->materialInfo->Init(device, sizeof(BaseMaterial::BasicMaterialStruct), &data, D3D11_USAGE_DYNAMIC, D3D11_CPU_ACCESS_WRITE);
	}

	RenderData GetRenderData(ID3D11DeviceContext* context) override
	{
		BaseMaterial::BasicMaterialStruct newData = {};
		for (size_t i = 0; i < 4; i++) {
			newData.diffuse[i] = color[i];
		}
		// Set texture count

		this->materialInfo->UpdateBuffer(context, &newData);

		return RenderData{
			.pixelShader {nullptr},
			.vertexShader {nullptr},
			.pixelBuffers { this->materialInfo.get() },
			.vertexBuffers {},
			.textures {
				this->diffuseTexture ? this->diffuseTexture->GetSrv() : nullptr,
			},
		};
	}
};