#pragma once

#include <d3d11.h>

#include <iostream>
#include <fstream>
#include <string>

#include <wrl/client.h>

enum class ShaderType
{
	VERTEX_SHADER,
	HULL_SHADER,
	DOMAIN_SHADER,
	GEOMETRY_SHADER,
	PIXEL_SHADER,
	COMPUTE_SHADER
};

class Shader
{
private:

	ShaderType type;

	union
	{
		Microsoft::WRL::ComPtr<ID3D11VertexShader> vertex = nullptr;
		Microsoft::WRL::ComPtr<ID3D11HullShader> hull;
		Microsoft::WRL::ComPtr<ID3D11DomainShader> domain;
		Microsoft::WRL::ComPtr<ID3D11GeometryShader> geometry;
		Microsoft::WRL::ComPtr<ID3D11PixelShader> pixel;
		Microsoft::WRL::ComPtr<ID3D11ComputeShader> compute;
	} shader;

	Microsoft::WRL::ComPtr<ID3DBlob> shaderBlob = nullptr;

	std::string shaderByteCode;

public:
	Shader();
	~Shader() = default;
	Shader(const Shader& other) = delete;
	Shader& operator=(const Shader& other) = delete;
	Shader(Shader&& other) = delete;
	Shader& operator=(Shader&& other) = delete;

	void Init(ID3D11Device* device, ShaderType shaderType, const char* csoPath);

	const void* GetShaderByteData() const;
	size_t GetShaderByteSize() const;
	std::string& GetShaderByteCode();

	void* GetShader();

	void BindShader(ID3D11DeviceContext* context) const;
};