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
	void AddSoundClipStandardFolder(std::string filename);
	void AddSoundClip(std::string path);

	SoundClip* GetSoundClipStandardFolder(std::string filename);
	SoundClip* GetSoundClip(std::string path);

	SoundSource soundSource; //remove!

private:
	MusicTrack currentMusicTrack;
	SoundBank soundBank;
};

