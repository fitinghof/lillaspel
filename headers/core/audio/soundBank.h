#pragma once
#include "al.h"
#include "alc.h"
#include <string>
#include <iostream>
#include <unordered_map>
#include "sndfile.h"
#include "soundClip.h"
#include "utilities/logger.h"

class SoundBank
{
public:
	SoundBank();
	~SoundBank();

	void Initialize(std::string pathToSoundFolder);
	void AddSoundClipStandardFolder(const std::string filename, const std::string id); //filename without the path to its folder
	void AddSoundClip(const std::string path, const std::string id); //full/relative path to soundfile
	std::string GetPathToSoundFolder();
	SoundClip* GetSoundClip(const std::string id);
	bool RemoveSoundClip(const std::string path);

private:
	std::unordered_map<std::string, SoundClip*> soundClips;
	std::string pathToSoundFolder = "";

	void CreateSoundBuffer(std::string filepath, std::string id);
	void DeleteSoundBuffer(std::string id);
};