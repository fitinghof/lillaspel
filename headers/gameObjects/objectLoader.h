#pragma once
#include <string>
#include <filesystem>
#include "rendering/vertex.h"
#include "gameObjects/mesh.h"
#include "rendering/vertexBuffer.h"
#include "rendering/indexBuffer.h"
#include "gameObjects/meshObjData.h"
#include "rendering/material.h"


struct MeshLoadData {
	Mesh mesh;
	std::vector<Material> materials;
	MeshObjData meshData;
};



class ObjectLoader
{
public:
	ObjectLoader(std::filesystem::path basePath = "../../assets");
	~ObjectLoader();

	/// <summary>
	/// Loads a gltf file 
	/// </summary>
	bool LoadGltf(std::filesystem::path localpath, std::vector<MeshLoadData>& meshLoadData , ID3D11Device* device);



private:
	std::filesystem::path basePath;
};