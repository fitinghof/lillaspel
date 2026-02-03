#pragma once
#include <vector>

#include "soundSource.h"

class SoundSourceManager
{
public:
	SoundSourceManager() = default;
	~SoundSourceManager() = default;

	SoundSource* GetSoundSource(int id);
	void AddSoundSource(SoundSource* speaker);
	void SetListenerPosition(float x, float y, float z);

private:
	int idCounter = 1;
	std::vector<SoundSource*> soundSources;
};