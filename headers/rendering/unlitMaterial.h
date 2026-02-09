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
	
	float[4] color = { 1, 1, 1, 1 };
	std::shared_ptr<Texture> diffuseTexture;

	std::unique_ptr<ConstantBuffer> materialInfo;

	UnlitMaterial(ID3D11Device* device) : BaseMaterial(device)
	{
		BaseMaterial::BasicMaterialStruct data = {};
		this->materialInfo = std::make_unique<ConstantBuffer>();
		this->materialInfo->Init(this->device.Get(), sizeof(BaseMaterial::BasicMaterialStruct), &data, D3D11_USAGE_DYNAMIC, D3D11_CPU_ACCESS_WRITE);
	}

	RenderData GetRenderData(ID3D11DeviceContext* context) override
	{
		BaseMaterial::BasicMaterialStruct newData;
		newData.diffuse = color;
		// Set texture count

		this->materialInfo->UpdateBuffer(context, &newData);

		// Actually return
	}
};