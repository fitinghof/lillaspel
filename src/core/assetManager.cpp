#include "core/assetManager.h"

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

	return false;
}

bool AssetManager::GetMesh(std::string identifier)
{
	return false;
}

bool AssetManager::loadNewGltf(std::string identifier) {
	std::vector<MeshLoadData> meshLoadData;
	
	this->objectLoader.LoadGltf(identifier, meshLoadData, this->d3d11Device);
	
	for (MeshLoadData& data : meshLoadData) 
	{
		this->meshes.insert({ data.mesh.GetName(), std::move(data.mesh)});
	}
}

MeshObjData AssetManager::GetTemplate(std::string identifier)
{
	return MeshObjData();
}

SoundClip* AssetManager::GetSoundClipStandardFolder(std::string filename)
{
	return this->soundBank.GetSoundClipStandardFolder(filename);
}

SoundClip* AssetManager::GetSoundClip(std::string path)
{
	return this->soundBank.GetSoundClip(path);
}
