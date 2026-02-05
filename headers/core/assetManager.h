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


class AssetManager
{
public:
	AssetManager(ID3D11Device* device);
	AssetManager() = default;
	~AssetManager() = default;

	Mesh* GetMeshPtr(std::string ident) { return ident != "" ? &this->meshes.at(ident) : nullptr; }
	Material* GetMaterialPtr(std::string ident) { return ident != "" ? &this->materials.at(ident) : nullptr; }

	void InitializeSoundBank(std::string pathToSoundFolder); //end the path with /
	void AddSoundClipStandardFolder(std::string filename);
	void AddSoundClip(std::string path);

	void setDevicePointer(ID3D11Device* device);

	bool GetMaterial(std::string identifier);
	bool GetMesh(std::string identifier);
	bool GetTexture(std::string identifier);
	MeshObjData GetMeshObjData(std::string identifier);


	SoundClip* GetSoundClipStandardFolder(std::string filename);
	SoundClip* GetSoundClip(std::string path);

private:
	MusicTrack currentMusicTrack;
	SoundBank soundBank;
	ObjectLoader objectLoader;

	std::unordered_map<std::string, Material> materials;
	std::unordered_map<std::string, Mesh> meshes;
	std::unordered_map<std::string, Texture> textures;
	std::unordered_map<std::string, MeshObjData> meshObjDataSets;

	bool LoadNewGltf(std::string identifier);
	
	ID3D11Device* d3d11Device = nullptr;

};

