#pragma once

#include "rendering/shader.h"
#include "rendering/constantBuffer.h"
#include <vector>
#include "utilities/logger.h"

class Material {
public:
	Material() = default;
	~Material() = default;

	void Init(std::shared_ptr<Shader>& vShader, std::shared_ptr<Shader>& pShader);

	std::shared_ptr<Shader> vertexShader;
	std::shared_ptr<Shader> pixelShader;

	std::vector<std::unique_ptr<ConstantBuffer>> vertexShaderBuffers;
	std::vector<std::unique_ptr<ConstantBuffer>> pixelShaderBuffers;
};