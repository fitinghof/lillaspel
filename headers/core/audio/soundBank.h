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

	std::string pathToSoundFolder;

	void Initialize(std::string pathToSoundFolder);
	void AddSoundClipStandardFolder(const std::string filename, const std::string id); //filename without the path to its folder
	void AddSoundClip(const std::string path, const std::string id); //full/relative path to soundfile
	SoundClip* GetSoundClipStandardFolder(const std::string filename);
	SoundClip* GetSoundClip(const std::string path);
	bool RemoveSoundClipStandardFolder(const std::string filename);
	bool RemoveSoundClip(const std::string relativePath);

private:
	std::unordered_map<std::string, SoundClip*> soundClips;

	void CreateSoundBuffer(std::string filepath, std::string id);
	void DeleteSoundBuffer(std::string id);
};