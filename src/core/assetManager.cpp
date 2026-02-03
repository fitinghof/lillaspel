#include "core/assetManager.h"

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

bool AssetManager::GetMaterial(std::string identifier)
{
	return false;
}

bool AssetManager::GetMesh(std::string identifier)
{
	return false;
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
