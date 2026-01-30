#pragma once
#include <fastgltf/core.hpp>
#include <fastgltf/tools.hpp>
#include <string>
#include <filesystem>
#include "rendering/vertex.h"
#include "gameObjects/mesh.h"
#include "rendering/vertexBuffer.h"
#include "rendering/indexBuffer.h"


//struct GltfLoadResult {
//public:
//	GltfLoadResult(Mesh&& m) : mesh(std::move(m)) {};
//	Mesh mesh;
//	//std::vector<Material> materials;
//};

class ObjectLoader
{
public:
	ObjectLoader();
	~ObjectLoader();
	Mesh LoadGltf(std::filesystem::path path, ID3D11Device* device);



private:

};