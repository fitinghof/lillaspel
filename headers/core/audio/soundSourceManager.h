#pragma once
#include <vector>

#include "soundSourceObject.h"

class SoundSourceManager
{
public:
	SoundSourceManager() = default;
	~SoundSourceManager() = default;

	SoundSourceObject* GetSoundSource(int id);
	void AddSoundSource(SoundSourceObject* speaker);
	void SetListenerPosition(float x, float y, float z);

private:
	int idCounter = 1;
	std::vector<SoundSourceObject*> soundSources;
};