#include "core/assetManager.h"

void AssetManager::InitializeSoundBank(std::string pathToSoundFolder)
{
	this->soundBank.Initialize(pathToSoundFolder);
}

void AssetManager::AddSoundClipStandardFolder(std::string standardFolder, std::string id)
{
	this->soundBank.AddSoundClipStandardFolder(standardFolder, id);
}

void AssetManager::AddSoundClip(std::string path, std::string id)
{
	this->soundBank.AddSoundClip(path, id);
}

SoundClip* AssetManager::GetSoundClipStandardFolder(std::string filename)
{
	return this->soundBank.GetSoundClipStandardFolder(filename);
}

SoundClip* AssetManager::GetSoundClip(std::string path)
{
	return this->soundBank.GetSoundClip(path);
}
