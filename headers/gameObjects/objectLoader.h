#pragma once
#include <string>
#include <filesystem>
#include "rendering/vertex.h"
#include "gameObjects/mesh.h"


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
	Mesh LoadGltf(std::filesystem::path path);



private:

};