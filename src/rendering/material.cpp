#include "rendering/material.h"

void Material::Init(std::shared_ptr<Shader>& vShader, std::shared_ptr<Shader>& pShader)
{
	this->vertexShader = vShader;
	this->pixelShader = pShader;
}
