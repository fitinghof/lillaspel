#pragma once
#include <vector>
#include <unordered_map>

#include "core/audio/soundEngine.h"

class AssetManager
{
public:
	AssetManager() = default;
	~AssetManager() = default;

	//Meshes
	//Materials
	//Textures
	//Shaders

	//Audio
	void InitializeSoundBank(std::string pathToSoundFolder); //end the path with /
	void AddSoundClipStandardFolder(std::string filename, std::string id);
	void AddSoundClip(std::string path, std::string id);
	std::string GetPathToSoundFolder();
	SoundClip* GetSoundClip(std::string id);

private:
	SoundBank soundBank;
};

