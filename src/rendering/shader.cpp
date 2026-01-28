#include "Shader.h"

Shader::Shader() : type(ShaderType::PIXEL_SHADER)
{
}

void Shader::Init(ID3D11Device* device, ShaderType shaderType, const char* csoPath)
{
	std::string shaderData;
	std::ifstream reader;
	reader.open(csoPath, std::ios::binary | std::ios::ate);
	if (!reader.is_open())
	{
		throw std::exception("Could not open shader file!");
	}

	reader.seekg(0, std::ios::end);
	shaderData.reserve(static_cast<unsigned int>(reader.tellg()));
	reader.seekg(0, std::ios::beg);

	shaderData.assign((std::istreambuf_iterator<char>(reader)),
		std::istreambuf_iterator<char>());

	HRESULT hr = 0;

	switch (shaderType)
	{
	case ShaderType::VERTEX_SHADER:
		hr = device->CreateVertexShader(shaderData.c_str(), shaderData.length(), nullptr, this->shader.vertex.GetAddressOf());
		break;
	case ShaderType::HULL_SHADER:
		hr = device->CreateHullShader(shaderData.c_str(), shaderData.length(), nullptr, this->shader.hull.GetAddressOf());
		break;
	case ShaderType::DOMAIN_SHADER:
		hr = device->CreateDomainShader(shaderData.c_str(), shaderData.length(), nullptr, this->shader.domain.GetAddressOf());
		break;
	case ShaderType::GEOMETRY_SHADER:
		hr = device->CreateGeometryShader(shaderData.c_str(), shaderData.length(), nullptr, this->shader.geometry.GetAddressOf());
		break;
	case ShaderType::PIXEL_SHADER:
		hr = device->CreatePixelShader(shaderData.c_str(), shaderData.length(), nullptr, this->shader.pixel..GetAddressOf());
		break;
	case ShaderType::COMPUTE_SHADER:
		hr = device->CreateComputeShader(shaderData.c_str(), shaderData.length(), nullptr, this->shader.compute.GetAddressOf());
		break;
	}

	if (FAILED(hr))
	{
		std::cerr << "Failed to create vertex shader!" << std::endl;
	}

	if (shaderType == ShaderType::VERTEX_SHADER) {
		this->shaderByteCode = shaderData;
	}

	shaderData.clear();
	reader.close();

	this->type = shaderType;
}

const void* Shader::GetShaderByteData() const
{
	return this->shaderByteCode.c_str();
}

size_t Shader::GetShaderByteSize() const
{
	return this->shaderByteCode.length();
}

std::string& Shader::GetShaderByteCode()
{
	return this->shaderByteCode;
}

void* Shader::GetShader()
{
	return this->shader.vertex;
}

void Shader::BindShader(ID3D11DeviceContext* context) const
{
	switch (this->type)
	{
	case ShaderType::VERTEX_SHADER:
		context->VSSetShader(this->shader.vertex.Get(), nullptr, 0);
		break;
	case ShaderType::HULL_SHADER:
		context->HSSetShader(this->shader.hull.Get(), nullptr, 0);
		break;
	case ShaderType::DOMAIN_SHADER:
		context->DSSetShader(this->shader.domain.Get(), nullptr, 0);
		break;
	case ShaderType::GEOMETRY_SHADER:
		context->GSSetShader(this->shader.geometry.Get(), nullptr, 0);
		break;
	case ShaderType::PIXEL_SHADER:
		context->PSSetShader(this->shader.pixel.Get(), nullptr, 0);
		break;
	case ShaderType::COMPUTE_SHADER:
		context->CSSetShader(this->shader.compute.Get(), nullptr, 0);
		break;
	}
}