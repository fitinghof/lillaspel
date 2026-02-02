#pragma once
#include <vector>
#include <unordered_map>

#include "../headers/core/audio/soundEngine.h"

class AssetManager
{
public:
	AssetManager() = default;
	~AssetManager() = default;

	void InitializeSoundBank(std::string pathToSoundFolder); //end the path with /
	void AddSoundClipStandardFolder(std::string filename, std::string id);
	void AddSoundClip(std::string path, std::string id);

	SoundClip* GetSoundClipStandardFolder(std::string filename);
	SoundClip* GetSoundClip(std::string path);

private:
	MusicTrack currentMusicTrack;
	SoundBank soundBank;
};

