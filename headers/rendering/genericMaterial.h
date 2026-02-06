#pragma once

#define NOMINMAX
#define WIN32_LEAN_AND_MEAN


#include "rendering/baseMaterial.h"

#include <memory>
#include "rendering/texture.h"


class GenericMaterial : public BaseMaterial {
public: 
	std::shared_ptr<Texture> diffuseTexture;
	std::shared_ptr<Texture> ambientTexture;
	std::shared_ptr<Texture> specularTexture;
	std::shared_ptr<Texture> normalTexture;

	RenderData GetRenderData() override {
		return RenderData{
			.pixelShader {nullptr},
			.vertexShader {nullptr},
			.pixelBuffers {},
			.vertexBuffers {},
			.textures {
				this->diffuseTexture ? this->diffuseTexture->GetSrv() : nullptr,
				this->ambientTexture ? this->ambientTexture->GetSrv() : nullptr,
				this->specularTexture ? this->specularTexture->GetSrv() : nullptr,
				this->normalTexture ? this->normalTexture->GetSrv() : nullptr,
			},
		};
	}
};