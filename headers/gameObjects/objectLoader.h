#pragma once
#include <string>
#include <filesystem>
#include "rendering/vertex.h"
#include "gameObjects/mesh.h"
#include "rendering/vertexBuffer.h"
#include "rendering/indexBuffer.h"
#include "gameObjects/meshObjData.h"
#include "rendering/material.h"
#include <fastgltf/core.hpp>


struct MeshLoadData {
	std::vector<std::shared_ptr<Mesh>> meshes;
	std::vector<std::shared_ptr<Material>> materials;
	std::vector<std::shared_ptr<Texture>> textures;
	std::vector<MeshObjData> meshData;
};



class ObjectLoader
{
public:
	ObjectLoader(std::filesystem::path basePath = "../../assets");
	~ObjectLoader();

	/// <summary>
	/// Loads a gltf file 
	/// </summary>
	bool LoadGltf(std::filesystem::path localpath, MeshLoadData& meshLoadData , ID3D11Device* device);



private:
	void LoadVerticiesAndNormals(const fastgltf::Asset& asset, const fastgltf::Primitive& primitive, std::vector<Vertex>& dest, uint32_t& offset);
	bool LoadIndices(fastgltf::Asset& asset, const fastgltf::Primitive& primitive, std::vector<uint32_t>& dest, uint32_t& offset);
	bool LoadUV(const fastgltf::Asset& asset, const fastgltf::Primitive& primitive, std::vector<Vertex>& dest, size_t offset);
	ID3D11ShaderResourceView* LoadTexture(const fastgltf::Asset& asset, const fastgltf::Primitive& primitive, ID3D11Device* device);

	std::filesystem::path basePath;
};