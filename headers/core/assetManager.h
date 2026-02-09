#pragma once
#include "gameObjects/mesh.h"
#include <vector>
#include <unordered_map>
#include "d3d11.h"

#include "gameObjects/meshObjData.h"

#include "../headers/core/audio/soundEngine.h"
#include "rendering/material.h"
#include "gameObjects/objectLoader.h"
#include "rendering/texture.h"
#include "rendering/shader.h"


class AssetManager
{
public:
	
	AssetManager(const AssetManager& assetManager) = delete;

	Mesh* GetMeshPtr(std::string ident) { return ident != "" ? this->meshes.at(ident).get() : nullptr; }
	BaseMaterial* GetMaterialPtr(std::string ident) { return ident != "" ? this->materials.at(ident).get() : nullptr; }
	std::weak_ptr<BaseMaterial> GetMaterialWeakPtr(std::string ident) { return ident != "" ? this->materials.at(ident) : nullptr; }

	void InitializeSoundBank(std::string pathToSoundFolder); //end the path with /
	void AddSoundClipStandardFolder(std::string filename, std::string id);
	void AddSoundClip(std::string path, std::string id);

	void SetDevicePointer(ID3D11Device* device);

	bool GetMaterial(std::string identifier);
	bool GetMesh(std::string identifier);
	bool GetTexture(std::string identifier);
	MeshObjData GetMeshObjData(std::string identifier);

	std::string GetPathToSoundFolder();
	SoundClip* GetSoundClip(std::string path);

	static AssetManager& GetInstance();

	/// <summary>
	/// Until we get automatic file loading of materials, shaders, textures etc, this is where it's done
	/// </summary>
	void CreateDefaultAssets();

	void AddShader(std::string identifier, std::shared_ptr<Shader> shader);
	std::shared_ptr<Shader> GetShaderPtr(std::string ident) { return ident != "" ? this->shaders.at(ident) : nullptr; }

private:
	SoundBank soundBank;
	ObjectLoader objectLoader;

	AssetManager() = default;
	~AssetManager() = default;

	std::unordered_map<std::string, std::shared_ptr<Shader>> shaders;
	std::unordered_map<std::string, std::shared_ptr<BaseMaterial>> materials;
	std::unordered_map<std::string, std::shared_ptr<Mesh>> meshes;
	std::unordered_map<std::string, std::shared_ptr<Texture>> textures;
	std::unordered_map<std::string, MeshObjData> meshObjDataSets;

	bool LoadNewGltf(std::string identifier);
	
	ID3D11Device* d3d11Device = nullptr;

	std::string getCleanPath(std::string pathToFix);
};

