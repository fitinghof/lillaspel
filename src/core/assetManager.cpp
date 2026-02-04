#include "core/assetManager.h"

void AssetManager::InitializeSoundBank(std::string pathToSoundFolder)
{
	this->soundBank.Initialize(pathToSoundFolder);
}

void AssetManager::AddSoundClipStandardFolder(std::string filename, std::string id)
{
	this->soundBank.AddSoundClipStandardFolder(this->soundBank.GetPathToSoundFolder() + filename, id);
}

void AssetManager::AddSoundClip(std::string path, std::string id)
{
	this->soundBank.AddSoundClip(path, id);
}

SoundClip* AssetManager::GetSoundClip(std::string id)
{
	return this->soundBank.GetSoundClip(id);
}
