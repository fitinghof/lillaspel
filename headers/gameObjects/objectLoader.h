#pragma once
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

	/// <summary>
	/// Loads a gltf file 
	/// </summary>
	bool LoadGltf(Mesh& mesh, std::filesystem::path path, ID3D11Device* device);



private:

};