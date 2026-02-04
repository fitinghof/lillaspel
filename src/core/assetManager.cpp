#include "core/assetManager.h"
#include "gameObjects/mesh.h"

AssetManager::AssetManager(ID3D11Device* device)
{
	this->setDevicePointer(device);
}

void AssetManager::InitializeSoundBank(std::string pathToSoundFolder)
{
	this->soundBank.Initialize(pathToSoundFolder);
}

void AssetManager::AddSoundClipStandardFolder(std::string standardFolder)
{
	this->soundBank.AddSoundClipStandardFolder(standardFolder);
}

void AssetManager::AddSoundClip(std::string path)
{
	this->soundBank.AddSoundClip(path);
}

void AssetManager::setDevicePointer(ID3D11Device* device)
{
	this->d3d11Device = device;
}

bool AssetManager::GetMaterial(std::string identifier)
{
	if (this->materials.find(identifier) != materials.end()) {
		return true;
	}
	else
	{
		if (!this->LoadNewGltf(identifier)) {
			return false;
		}
	}
	return true;
}

bool AssetManager::GetMesh(std::string identifier)
{
	if (this->meshes.find(identifier) != meshes.end()) {
		return true;
	}
	else
	{
		if (!this->LoadNewGltf(identifier)) {
			return false;
		}
	}
	return true;
}

bool AssetManager::GetTexture(std::string identifier)
{
	if (this->textures.find(identifier) != textures.end()) {
		return true;
	}
	else
	{
		if (!this->LoadNewGltf(identifier)) {
			return false;
		}
	}
	return true;
}

bool AssetManager::LoadNewGltf(std::string identifier) {
	MeshLoadData meshLoadData;
	
	bool objectLoaded = this->objectLoader.LoadGltf(identifier, meshLoadData, this->d3d11Device);
	
	if (!objectLoaded) {
		return false;
	}
	
	for (Mesh& data : meshLoadData.meshes) 
	{
		this->meshes.emplace(data.GetName(), std::move(data));
	}
	for (Material& data : meshLoadData.materials) 
	{
		this->materials.emplace(data.identifier, std::move(data));
	}
	for (Texture& data : meshLoadData.textures) 
	{
		this->textures.emplace(data.GetIdentifier(), std::move(data));
	}
	for (MeshObjData& data : meshLoadData.meshData) 
	{
		this->meshObjDataSets.emplace(identifier, std::move(data));
	}

	return true;
}

MeshObjData AssetManager::GetMeshObjData(std::string identifier)
{
	if (this->meshObjDataSets.find(identifier) != meshObjDataSets.end()) {
		return meshObjDataSets.at(identifier);
	}
	else
	{
		this->LoadNewGltf(identifier);
		return meshObjDataSets.at(identifier);
	}
}

SoundClip* AssetManager::GetSoundClipStandardFolder(std::string filename)
{
	return this->soundBank.GetSoundClipStandardFolder(filename);
}

SoundClip* AssetManager::GetSoundClip(std::string path)
{
	return this->soundBank.GetSoundClip(path);
}
